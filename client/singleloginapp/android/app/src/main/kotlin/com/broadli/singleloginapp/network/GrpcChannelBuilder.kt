package com.broadli.singleloginapp.network

import android.annotation.TargetApi
import android.net.SSLCertificateSocketFactory
import android.os.Build
import io.grpc.ManagedChannel
import io.grpc.ManagedChannelBuilder
import io.grpc.okhttp.NegotiationType
import io.grpc.okhttp.OkHttpChannelBuilder
import java.io.InputStream
import java.security.KeyStore
import java.security.cert.CertificateFactory
import java.security.cert.X509Certificate
import javax.net.ssl.SSLContext
import javax.net.ssl.SSLSocketFactory
import javax.net.ssl.TrustManager
import javax.net.ssl.TrustManagerFactory

object GrpcChannelBuilder {
    /**
     * 用来生成ManagedChannel的
     *
     * @param host                    ip地址
     * @param port                    端口号
     * @param serverHostOverride      这个不知道是啥，是服务器端给我的
     * @param useTls                  是否使用tls加密 ，ture代表使用https加密，false代表不使用，
     * @param testCa                  证书的流
     * @param androidSocketFactoryTls 不知道是啥，传null
     * @return
     */
    fun build(host: String?, port: Int, serverHostOverride: String?,
              useTls: Boolean, testCa: InputStream?, androidSocketFactoryTls: String?): ManagedChannel {
        val channelBuilder = ManagedChannelBuilder.forAddress(host, port)
        if (serverHostOverride != null) { // Force the hostname to match the cert the server uses.
            channelBuilder.overrideAuthority(serverHostOverride)
        }
        if (useTls) {
            try {
                val factory: SSLSocketFactory
                factory = androidSocketFactoryTls?.let { getSslCertificateSocketFactory(testCa, it) }
                        ?: getSslSocketFactory(testCa)
                (channelBuilder as OkHttpChannelBuilder).negotiationType(NegotiationType.TLS)
                channelBuilder.sslSocketFactory(factory)
            } catch (e: Exception) {
                throw RuntimeException(e)
            }
        } else { // 是否使用tls加密，true不使用
            channelBuilder.usePlaintext(true)
        }
        return channelBuilder.build()
    }

    @Throws(Exception::class)
    private fun getSslSocketFactory(testCa: InputStream?): SSLSocketFactory {
        if (testCa == null) {
            return SSLSocketFactory.getDefault() as SSLSocketFactory
        }
        val context = SSLContext.getInstance("TLS")
        context.init(null, getTrustManagers(testCa), null)
        return context.socketFactory
    }

    @TargetApi(14)
    @Throws(Exception::class)
    private fun getSslCertificateSocketFactory(
            testCa: InputStream?, androidSocketFatoryTls: String): SSLCertificateSocketFactory {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.ICE_CREAM_SANDWICH /* API level 14 */) {
            throw RuntimeException(
                    "android_socket_factory_tls doesn't work with API level less than 14.")
        }
        val factory = SSLCertificateSocketFactory.getDefault(5000 /* Timeout in ms*/) as SSLCertificateSocketFactory
        // Use HTTP/2.0
        val h2 = "h2".toByteArray()
        val protocols = arrayOf(h2)
        if (androidSocketFatoryTls == "alpn") {
            val setAlpnProtocols = factory.javaClass.getDeclaredMethod("setAlpnProtocols", Array<ByteArray>::class.java)
            setAlpnProtocols.invoke(factory, *arrayOf<Any>(protocols))
        } else if (androidSocketFatoryTls == "npn") {
            val setNpnProtocols = factory.javaClass.getDeclaredMethod("setNpnProtocols", Array<ByteArray>::class.java)
            setNpnProtocols.invoke(factory, *arrayOf<Any>(protocols))
        } else {
            throw RuntimeException("Unknown protocol: $androidSocketFatoryTls")
        }
        if (testCa != null) {
            factory.setTrustManagers(getTrustManagers(testCa))
        }
        return factory
    }

    @Throws(Exception::class)
    private fun getTrustManagers(testCa: InputStream): Array<TrustManager> {
        val ks = KeyStore.getInstance(KeyStore.getDefaultType())
        ks.load(null)
        val cf = CertificateFactory.getInstance("X.509")
        val cert = cf.generateCertificate(testCa) as X509Certificate
        val principal = cert.subjectX500Principal
        ks.setCertificateEntry(principal.getName("RFC2253"), cert)
        // Set up trust manager factory to use our key store.
        val trustManagerFactory = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm())
        trustManagerFactory.init(ks)
        return trustManagerFactory.trustManagers
    }
}