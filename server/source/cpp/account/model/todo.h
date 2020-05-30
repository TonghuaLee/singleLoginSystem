
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
		Todo(int cid, string title, int uid);

		int getCid();
		void setCid(int uid);

		string getContent();
		void setContent(string content);

		int getTid();
		void setTid(int uid);

		int getStatus();
		void setStatus(int status);

	private:
		int cid;
		string content;
		int tid;
		int status;
	};

} // namespace my_model