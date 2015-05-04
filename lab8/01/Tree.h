/*
 * Metody Programowania - Laboratorium 8 - Program 01
 * Implementacja klas: drzewo binarne z powrotem
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include "List.h"

#ifndef __TREE_H__
#define __TREE_H__

namespace Tree
{
	template <class T>
	class BinaryReturn;

	namespace Node
	{
		class Key	// klucz jednoznacznie identyfikujacy wezel w drzewie
		{
		public:
			int L;		// ilosc lewych rozgalezien
			int R;		// ilosc prawych rozgalezien
			int Depth;	// glebokosc w drzewie

			Key();
			Key(int L, int R, int Depth);
			~Key();
			int operator==(Key &k);
		};

		template <class T>
		class BinaryReturn		// wezel drzewa binarnego z powrotem
		{
			friend class Tree::BinaryReturn<T>;

		private:
			BinaryReturn<T> *parent;
			T data;
			BinaryReturn<T> *left;
			BinaryReturn<T> *right;

		public:
			Key *key;
			BinaryReturn(T data);
			~BinaryReturn();
			BinaryReturn<T> *Parent(void);
			BinaryReturn<T> *Left(void);
			BinaryReturn<T> *Right(void);
			T *getData(void);
			int operator==(BinaryReturn<T> &n);
		};
	}

	template <class T>
	class BinaryReturn		// drzewo binarne z powrotem
	{
	private:
		Node::BinaryReturn<T> *root;
		Node::BinaryReturn<T> *current;

	public:
		BinaryReturn();
		~BinaryReturn();
		Node::BinaryReturn<T> *getRoot(void);
		void setRoot(Node::BinaryReturn<T> *node);
		Node::BinaryReturn<T> *Reset(void);
		Node::BinaryReturn<T> *Current(void);
		Node::BinaryReturn<T> *Left(void);
		Node::BinaryReturn<T> *Right(void);
		Node::BinaryReturn<T> *Parent(void);
		Node::Key *appendRight(Node::BinaryReturn<T> *node);
		Node::Key *appendLeft(Node::BinaryReturn<T> *node);
		List::List<T> *pathTo(Node::Key *key);
	};
}

// ---------------------------------------------- class Tree::Node::Key
Tree::Node::Key::Key()
{
	this->L = 0;
	this->R = 0;
	this->Depth = 0;
}

Tree::Node::Key::Key(int L, int R, int Depth)
{
	this->L = L;
	this->R = R;
	this->Depth = Depth;
}

Tree::Node::Key::~Key()
{
}

int Tree::Node::Key::operator==(Tree::Node::Key &k)
{
	return ((this->L == k.L) && (this->R = k.R) && (this->Depth == k.Depth));
}

//----------------------------------------------- class Tree::Node::BinaryReturn
template <class T>
Tree::Node::BinaryReturn<T>::BinaryReturn(T data)
{
	this->data = data;
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
	this->key = new Tree::Node::Key;
}

template <class T>
Tree::Node::BinaryReturn<T>::~BinaryReturn()
{
	delete this->key;
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::Node::BinaryReturn<T>::Parent(void)
{
	return this->parent;
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::Node::BinaryReturn<T>::Left(void)
{
	return this->left;
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::Node::BinaryReturn<T>::Right(void)
{
	return this->right;
}

template <class T>
T *Tree::Node::BinaryReturn<T>::getData(void)
{
	return &this->data;		// zwracam wskaznik do danych, bo dane moga byc typu zlozonego
}

template <class T>
int Tree::Node::BinaryReturn<T>::operator==(Tree::Node::BinaryReturn<T> &n)
{
	return (this->key == n.key);
}

//----------------------------------------------- class Tree::BinaryReturn
template <class T>
Tree::BinaryReturn<T>::BinaryReturn()
{
	this->root = NULL;
	this->current = NULL;
}

template <class T>
Tree::BinaryReturn<T>::~BinaryReturn()
{
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::BinaryReturn<T>::getRoot(void)
{
	return this->root;
}

template <class T>
void Tree::BinaryReturn<T>::setRoot(Tree::Node::BinaryReturn<T> *node)
{
	this->root = node;
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::BinaryReturn<T>::Reset(void)
{
	this->current = this->root;
	return this->Current();
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::BinaryReturn<T>::Current(void)
{
	return this->current;
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::BinaryReturn<T>::Left(void)
{
	this->current = this->current->Left();
	return this->Current();
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::BinaryReturn<T>::Right(void)
{
	this->current = this->current->Right();
	return this->Current();
}

template <class T>
Tree::Node::BinaryReturn<T> *Tree::BinaryReturn<T>::Parent(void)
{
	this->current = this->current->Parent();
	return this->Current();
}

template <class T>
Tree::Node::Key *Tree::BinaryReturn<T>::appendRight(Node::BinaryReturn<T> *node)
{
	this->current->right = node;
	node->parent = this->Current();
	Tree::Node::Key *k = this->Current()->key;
	node->key = new Tree::Node::Key(k->L, k->R+1, k->Depth+1);
	return node->key;
}

template <class T>
Tree::Node::Key *Tree::BinaryReturn<T>::appendLeft(Node::BinaryReturn<T> *node)
{
	this->current->left = node;
	node->parent = this->Current();
	Tree::Node::Key *k = this->Current()->key;
	node->key = new Tree::Node::Key(k->L+1, k->R, k->Depth+1);
	return node->key;
}

template <class T>
List::List<T> *Tree::BinaryReturn<T>::pathTo(Node::Key *key)
{
	// TODO: Sciezka do wezla
}

#endif /* __TREE_H__ */