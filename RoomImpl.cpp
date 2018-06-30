#include "Chat.h"

using namespace Chat;
using namespace std;

class RoomImpl : public Room {
    public:
	RoomImpl(const string &n) : name(n) {}
	virtual string getName(const ::Ice::Current&) override;
	virtual UserList getUsers(const ::Ice::Current&) override;
        virtual void AddUser(const UserPrx&, const ::Ice::Current&) override;
	virtual void SendMessage(const UserPrx&, const string&, const ::Ice::Current&) override;
        virtual void Destroy(const ::Ice::Current&) override;
	virtual void LeaveRoom(const UserPrx&, const ::Ice::Current&) override;
    private:
	string name;
	UserList userList;
};

    string RoomImpl::getName(const ::Ice::Current&) {
        return name;
    }

    UserList RoomImpl::getUsers(const ::Ice::Current&) {
        return userList;
    }

    void RoomImpl::AddUser(const UserPrx& user, const ::Ice::Current& ) {
        userList.push_back(user);
    }
    void RoomImpl::SendMessage(const UserPrx& user, const string& message, const ::Ice::Current&){
	for (int i=0; i<userList.size(); i++){
		userList.at(i)->SendMessageToRoom(name, user, message);	
	}
    }
    void RoomImpl::Destroy(const ::Ice::Current&) {
    	userList.clear();
    }
    void RoomImpl::LeaveRoom(const UserPrx& user, const ::Ice::Current&) {
	int index;
   	for (int i=0; i<userList.size(); i++) {
        	if(userList.at(i)->getName()==user->getName())
		{
			index=i;
		}
	}
	userList.erase(userList.begin()+index);
    }

