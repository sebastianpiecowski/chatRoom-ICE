#include <Ice/Ice.h>
#include "Chat.h"
#include "RoomImpl.cpp"

using namespace Chat;
using namespace std;
int port=10030;

class IServer : public Server {
    public:
        virtual RoomPrx CreateRoom(const string&, const ::Ice::Current&) override;
	virtual void SendPriv(const UserPrx&, const string&, const string&, const ::Ice::Current&) override;
	virtual UserList getUsers(const ::Ice::Current&) override;
        virtual RoomList getRooms(const ::Ice::Current&) override;
        virtual RoomPrx FindRoom(const string&, const ::Ice::Current&) override;
	virtual void RegisterUser(const UserPrx&, const ::Ice::Current&)override;
	virtual void Quit(const UserPrx&, const ::Ice::Current&) override;
	virtual int getPort(const ::Ice::Current&);
    private:
        RoomList roomList;
	UserList userList;
	Ice::ObjectAdapterPtr adapter;
	Ice::CommunicatorPtr ic;
};
string toString(int a) {
	string tmp;
	sprintf((char*)tmp.c_str(), "%d", a);
	string str = tmp.c_str();
	return str;
}
    RoomPrx IServer::CreateRoom(const string& name, const ::Ice::Current&) {
	ic = Ice::initialize();
	for (int i=0; i<roomList.size(); i++) {
		if (roomList.at(i)->getName() == name) {
			throw RoomAlreadyExists();
		}         
        }      	
        RoomPtr object = new RoomImpl(name);
	++port;
	
	
	adapter = ic->createObjectAdapterWithEndpoints("SimpleRoom" + name, "default -p " + toString(port));
        RoomPrx room = RoomPrx::uncheckedCast(adapter->addWithUUID(object));
	adapter->activate();
        roomList.push_back(room);
        cout << "Room created" << endl;
        return room;
    }
    void IServer::Quit(const UserPrx& who, const ::Ice::Current&){
	for (int i=0; i<userList.size(); i++){
		if(userList.at(i)->getName()==who->getName()) {
			userList.erase(userList.begin()+i);
		}
	}
    }
    void IServer::SendPriv(const UserPrx& who, const string& to, const string& message, const ::Ice::Current&){
	bool sended=false;	
	for (int i=0; i<userList.size(); i++){
		if(userList.at(i)->getName()==to){
			userList.at(i)->SendPriv(who,message);			
			sended=true;
		}		
	}
	if(sended==false) {
		throw UserNotExists();	
	} 
    }

    RoomList IServer::getRooms(const ::Ice::Current&) {
        return roomList;
    }
	
    UserList IServer::getUsers(const ::Ice::Current&) {
    	return userList;
    }
    RoomPrx IServer::FindRoom(const string& name, const ::Ice::Current& ) {
        for (int i=0; i<roomList.size(); i++) {
            if (roomList.at(i)->getName() == name) {
                return roomList.at(i);
            }
        }
        throw RoomNotExists();   
    }
    void IServer::RegisterUser(const UserPrx& user, const ::Ice::Current&) {
	for (int i=0; i<userList.size(); i++) {
            if (userList.at(i)->getName() == user->getName()) {
                throw UserAlreadyExists();
            }
        }
	userList.push_back(user);
    }
    int IServer::getPort(const ::Ice::Current&){
	return ++port; 
    }

    
int main(int argc, char* argv[]) {
    Ice::CommunicatorPtr iceCommunicator;
    try {
        iceCommunicator = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter = iceCommunicator->createObjectAdapterWithEndpoints("ServerAdapter", "default -p 10000");
        Ice::ObjectPtr object = new IServer();
        adapter->add(object, iceCommunicator->stringToIdentity("Server"));
        adapter->activate();
        iceCommunicator->waitForShutdown();
    } catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
