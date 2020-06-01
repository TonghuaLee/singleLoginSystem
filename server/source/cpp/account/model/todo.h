
#include <string>

using namespace std;

namespace my_model
{

	/**
     *  Todo Model
     **/
	class Todo
	{
	public:
		Todo(int tid, string title, int uid, int cid, int status);

		int getCid();
		void setCid(int cid);

		string getContent();
		void setContent(string content);

		int getTid();
		void setTid(int tid);

		int getUid();
		void setUid(int uid);

		int getStatus();
		void setStatus(int status);

	private:
		int cid;
		string content;
		int tid;
		int uid;
		int status;
	};

} // namespace my_model