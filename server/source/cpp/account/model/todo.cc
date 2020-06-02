
#include "todo.h"

using namespace my_model;
using namespace std;

Todo::Todo(int tid, string content, int uid, int cid, int status) : tid(tid), content(content), uid(uid), cid(cid), status(status)
{
}

int Todo::getCid()
{
	return cid;
}
void Todo::setCid(int cid)
{
	this->cid = cid;
}

int Todo::getTid()
{
	return tid;
}
void Todo::setTid(int tid)
{
	this->tid = tid;
}

int Todo::getStatus()
{
	return status;
}
void Todo::setStatus(int status)
{
	this->status = status;
}

string Todo::getContent()
{
	return content;
}
void Todo::setContent(string content)
{
	this->content = content;
}
