class Group;
class Object;
class OBJLoader;

class OBJLoader{
private:
	//LinkedList of Objects
public:
	OBJLoader(char* fileName);
	~OBJLoader();
};
class Object{//LinkedList of Groups
private:
public:
	Object(char* oName);
	~Object();
};

class Group{
private:
	//Vertex List }
	//Normal List } should probably be some sort of Array
	//Display List, can be Linked List
	//Vector of Material Coefficients
public:
	Group(char* gName);
	~Group();
};