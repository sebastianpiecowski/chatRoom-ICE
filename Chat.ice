module Chat {

exception UserAlreadyExists {};
exception RoomAlreadyExists {};
exception RoomNotExists {};
exception UserNotExists {};

interface User;
interface Room;
interface Server;
sequence<User*> UserList;
sequence<Room*> RoomList;
interface Room {
  string getName();
  UserList getUsers();
  void AddUser(User* who);
  void SendMessage(User* who, string message);
  void Destroy();
  void LeaveRoom(User* who);
};
interface User {
  void SendMessageToRoom(string room, User* who, string message);
  void SendPriv(User* who, string message) throws UserNotExists;
  string getName();
};
interface Server {
  Room* CreateRoom(string name) throws RoomAlreadyExists;
  RoomList getRooms(); 
  UserList getUsers();
  void SendPriv(User*who, string to, string message) throws UserNotExists;
  Room* FindRoom(string name) throws RoomNotExists;
  void RegisterUser(User* who) throws UserAlreadyExists;
  void Quit(User*who);
  int getPort();
};






};
