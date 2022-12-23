#include "LinkedList.h"
template<typename T>
// Constructor
LinkedList<T>::LinkedList()
{
	// Set all to nullptr
	head = nullptr;
	tail = nullptr;
	temp = nullptr;
}

template<typename T>
// Destructor
LinkedList<T>::~LinkedList()
{
	Node<T>* temp = nullptr;
	while (head != nullptr)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

template<typename T>
void LinkedList<T>::AddFirst(T data)
{
	if (head == nullptr)
	{
		AddLast(data);
		return;
	}
	Node<T>* temp = new Node<T>();
	temp->data = data;
	// Readjusting List
	temp->next = head;
	head->prev = temp;
	head = temp;
	return;
}

template<typename T>
void LinkedList<T>::AddLast(T data)
{
	Node<T>* temp = new Node<T>();
	temp->data = data;
	// Checks if head is empty
	if (head == nullptr || head->next == nullptr)
	{
		head = temp;
		tail = temp;
		return;
	}
	// Readjusting List
	tail->next = temp;
	temp->prev = tail;
	tail = temp;
	return;
}

template<typename T>
Node<T>* LinkedList<T>::FindNode(Node<T>* nodeToFind)
{

}
