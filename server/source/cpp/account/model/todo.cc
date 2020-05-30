
#include "todo.h"

using namespace my_model;
using namespace std;

Todo::Todo(int tid, string content, int cid, int status) : tid(tid), content(content), cid(cid), status(status)
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
	return tid;
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
