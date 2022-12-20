#pragma once

// Linked List Class
template<typename T>
struct Node
{
	Node<T>* next;
	Node<T>* prev;
	T data;
};

// Doubly LinkedList
template<typename T>
class LinkedList
{
public:
	// Head Node
	Node<T>* head;
	// Tail Node
	Node<T>* tail;

	// Constructor
	LinkedList();
	// Destructor
	~LinkedList();

	/// <summary>
	/// Adds new Node* containing data at the start of the Linked List.
	/// </summary>
	/// <param name="data"></param>
	void AddFirst(T data);

	/// <summary>
	/// Adds new Node* containing data at the end of the Linked List.
	/// </summary>
	/// <param name="data"></param>
	void AddLast(T data);

	/// <summary>
	/// Adds new Node* after existing Node*.
	/// </summary>
	/// <param name="existingNode"></param>
	/// <param name="data"></param>
	void AddAfter(Node<T>* existingNode, T data);


	/// <summary>
	/// Recursively finds the Node* you are looking for in the Linked List.
	/// </summary>
	/// <param name="nodeToFind"></param>
	/// <returns>Returns NULL if nodetoFind cannot be found else returns found node.</returns>
	Node<T>* Find(Node<T>* nodeToFind);
private:
	Node<T>* temp;
};
