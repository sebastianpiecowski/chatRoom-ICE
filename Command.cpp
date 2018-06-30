#include <Ice/Ice.h>
#include "Chat.h"
#include "UserImpl.cpp"
#include "RoomImpl.cpp"
#include <vector>
using namespace std;
using namespace Chat;

namespace Command {
class Client {
    public:
        Client(const string&);
        void createRoom() const;
        void printListAllRooms() const;
        void joinToRoom();
        void printUsersInRoom() const;
        void leaveRoom();
        void sendPrivateMessageToUser() const;
        void sendMessageToRoom() const;
        ~Client();
    private:
        string username;
        UserPrx user;
        ServerPrx server;
        Ice::CommunicatorPtr iceCommunicator;
        Ice::ObjectAdapterPtr adapter;
        RoomList roomList;
        void createUser();
        string getNameOfTheRoom() const;
        UserList getUsersInRoom() const;
	UserList getUsersOnServer() const;
}; 
string toString(int a) {
	string tmp;
	sprintf((char*)tmp.c_str(), "%d", a);
	string str = tmp.c_str();
	return str;
}
void Client::createUser() {
        UserPtr object = new IUser(username);
        adapter = iceCommunicator->createObjectAdapterWithEndpoints("User" + username, "default -p " + toString(server->getPort()));
        user = UserPrx::uncheckedCast(adapter->addWithUUID(object));
        adapter->activate();
	server->RegisterUser(user);

    }

    Client::Client(const string& name) : username(name) {
        try {
            iceCommunicator = Ice::initialize();
            Ice::ObjectPrx base = iceCommunicator->stringToProxy("Server:default -p 10000");
            server = ServerPrx::checkedCast(base);
            if (!server)
                throw "Invalid proxy";
        } catch (const Ice::Exception& ex) {
            cerr << ex << endl;
        } catch (const char* msg) {
            cerr << msg << endl;
        }
        createUser();
    }

    Client::~Client() {
        for (int i=0; i<roomList.size(); i++) {
            roomList.at(i)->LeaveRoom(user);
	    
        }
	server->Quit(user);
        if (iceCommunicator)
            iceCommunicator->destroy();
	    adapter->destroy();
    }

    void Client::createRoom() const {
        string roomName;
        cout << "Enter name for new room " << endl;
        cin >> roomName;
        cin.ignore(1000, '\n');
        try {
            server->CreateRoom(roomName);
        } catch (const Chat::RoomAlreadyExists& ex) {
            cerr << "Room with that name already exists." << endl;
        } catch (const Ice::UnknownException& ex) {
            cerr << "Cannot create" << endl;
        }
    }

    void Client::printListAllRooms() const {
        RoomList rooms = server->getRooms();
        cout << "Available rooms are: " << endl;
        for (int i=0; i<rooms.size(); i++) {
            cout << rooms.at(i)->getName() << endl;
        }
        cout << endl;
    }

    void Client::joinToRoom() {
        string name = getNameOfTheRoom();
        try {
            RoomPrx room = server->FindRoom(name);
            room->AddUser(user);
            roomList.push_back(room);
        } catch (const RoomNotExists& ex) {
            cerr << "There is no such room" << endl;
        } catch (const UserAlreadyExists& ex) {
            cerr << "Such userr already exist" << endl;
        } catch (const Ice::UnknownException& ex) {
            cerr << "Operation couldn't be realized." << endl;
        }
    }

    void Client::printUsersInRoom() const {
        try {
            UserList users = getUsersInRoom();
            cout << "Users available in room " << endl;
            for (int i=0; i<users.size(); i++) {
                cout << users.at(i)->getName() << endl;
            }
        } catch (const Ice::UnknownException& ex) {
            cerr << "Operation couldn't be realized." << endl;
        }
    }

    UserList Client::getUsersInRoom() const {
        string roomName = getNameOfTheRoom();
        try {
            RoomPrx room = server->FindRoom(roomName);
            UserList users = room->getUsers();
            return users;
        } catch (const RoomNotExists& ex) {
            cerr << "There is no such room" << endl;
        } catch (Ice::UnknownException& ex) {
            cerr << ex << endl;
        }
        return UserList();
    }

    string Client::getNameOfTheRoom() const {
        string roomName;
        cout << "Enter the name of the room:" << endl;
        cin >> roomName;
        cin.ignore(1000, '\n');
        return roomName;
    }

    void Client::leaveRoom() {
        string roomName = getNameOfTheRoom();
	std::vector<RoomPrx> myvector;
        for (int i=0; i<roomList.size(); i++){
            if (roomList.at(i)->getName() == roomName) {
                try {
	       	   
                    roomList.at(i)->LeaveRoom(user);
		    roomList.erase(roomList.begin()+i);		
                    return;
                } catch (UserNotExists& ex) {
                    cerr << "You not belong to this room!" << endl;
                } catch (Ice::UnknownException& ex) {
                    cerr << ex << endl;
                }
            }
        }
        cerr << "You were not joined to room " << roomName << endl;
    }

    void Client::sendPrivateMessageToUser() const {
        string targetUsername;
        UserList usersAvailable;
        try {
            usersAvailable = server->getUsers();
        } catch (RoomNotExists& ex) {
            cerr << ex << endl;
            return;
        }
        cout << "Enter the name of the user you want to write to" << endl;
        cin >> targetUsername;
        cin.ignore(1000, '\n');
        for(int i=0; i<usersAvailable.size(); i++) {
            if (usersAvailable.at(i)->getName() == targetUsername) {
                string message;
                cout << "Enter the content of message you want to send to" << endl;
                getline(cin, message);
                usersAvailable.at(i)->SendPriv(user, message);
                return;
            }
        }
        cerr << "No such user found. Sorry." << endl;
    }

    void Client::sendMessageToRoom() const {
     
        string targetRoom = getNameOfTheRoom();
	
        for (int i=0; i<roomList.size(); i++) {
            if (roomList.at(i)->getName() == targetRoom) {
                try {
                    string content;
                    cout << "Please, enter message you want to send " << endl;
                    getline(cin, content);
                    roomList.at(i)->SendMessage(user, content);
                    return;
                } catch (UserNotExists& ex) {
                    cerr << "You not belong to this room!" << endl;
                    return;
                }
            }
        }
        cerr << "You are not joined to room " << targetRoom << endl;
    }
}

