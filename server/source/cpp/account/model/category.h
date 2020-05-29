
#include <string>

using namespace std;

namespace my_model{
	
    /**
     *  用户账号Model
     **/
	class Category{
	public:
		Category(int cid, string title, int uid);

		int getCid();
		void setCid(int uid);

		string getTitle();
		void setTitle(string account);

		int getUid();
		void setUid(int uid);

	private:
		int cid;
		string title;
		int uid;
	};

}