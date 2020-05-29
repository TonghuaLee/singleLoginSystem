
#include "category.h"

using namespace my_model;
using namespace std;

Category::Category(int cid, string title, int uid) : cid(cid), title(title), uid(uid) {

}

int Category::getCid()
{
	return cid;
}
void Category::setCid(int cid)
{
	this->cid = cid;
}

int Category::getUid(){
	return uid;
}
void Category::setUid(int uid){
	this->uid = uid;
}

string Category::getTitle(){
	return title;
}
void Category::setTitle(string title){
	this->title = title;
}

