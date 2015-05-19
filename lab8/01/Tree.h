/*
 * Metody Programowania - Laboratorium 8-9
 * Implementacja klas: drzewo binarne z powrotem
 * Grzegorz Kowalski, 12i
 * wersja 1 | 05.2015
 */

#include <stdio.h>
#include "List.h"

#ifndef __TREE_H__
#define __TREE_H__

#define BITMAP_RIGHT(b, i)  ((b) & ~(1 << (i)))		// czysci bit i
#define BITMAP_LEFT(b, i)	((b) | 1 << (i))		// ustawia bit i
#define BITMAP_CHECK(b, i)	(((b) >> (i)) & 1)		// sprawdza wartosc bitu i

namespace Tree
{
	template <class T>
	class BinaryReturn;

	namespace Node
	{
		class Key	// klucz jednoznacznie identyfikujacy wezel w drzewie
		{
		public:
			unsigned int Bitmap;		// mapa bitowa rozgalezien
			int Depth;		// glebokosc w drzewie

			Key();
			Key(unsigned int Bitmap, int Depth);
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

		void setCurrent(Node::BinaryReturn<T> *node);
		void updateBelow(bool is_left);

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
		Node::Key *appendRight(BinaryReturn<T> *tree);
		Node::Key *appendLeft(BinaryReturn<T> *tree);
		List::List<T> *pathTo(Node::Key *key);
	};
}

// ---------------------------------------------- class Tree::Node::Key
Tree::Node::Key::Key()
{
	this->Bitmap = 0;
	this->Depth = 0;
}

Tree::Node::Key::Key(unsigned int Bitmap, int Depth)
{
	this->Bitmap = Bitmap;
	this->Depth = Depth;
}

Tree::Node::Key::~Key()
{
}

int Tree::Node::Key::operator==(Tree::Node::Key &k)
{
	return ((this->Bitmap == k.Bitmap) && (this->Depth == k.Depth));
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
	if(this->left) delete this->left;
	if(this->right) delete this->right;
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
void Tree::BinaryReturn<T>::setCurrent(Tree::Node::BinaryReturn<T> *node)
{
	this->current = node;
}

template <class T>
void Tree::BinaryReturn<T>::updateBelow(bool is_left)
{
	this->Current()->key->Bitmap = is_left ?	// aktualizacja klucza w aktualnym wezle
								   BITMAP_LEFT(this->Current()->Parent()->key->Bitmap, this->Current()->Parent()->key->Depth+1) :
								   BITMAP_RIGHT(this->Current()->Parent()->key->Bitmap, this->Current()->Parent()->key->Depth+1);
	this->Current()->key->Depth = this->Current()->Parent()->key->Depth+1;

	if(this->Current()->Left())	// aktualizacja kluczy pod spodem
	{
		this->Left();
		this->updateBelow(true);
	}
	if(this->Current()->Right())
	{
		this->Right();
		this->updateBelow(false);
	}

	this->Parent();		// powrot
}

template <class T>
Tree::BinaryReturn<T>::BinaryReturn()
{
	this->root = NULL;
	this->current = NULL;
}

template <class T>
Tree::BinaryReturn<T>::~BinaryReturn()
{
	delete this->root;
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
	node->key->Bitmap = 0;
	node->key->Depth = 0;
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
Tree::Node::Key *Tree::BinaryReturn<T>::appendRight(Tree::Node::BinaryReturn<T> *node)
{
	this->current->right = node;
	node->parent = this->Current();
	Tree::Node::Key *k = this->Current()->key;
	node->key->Bitmap = BITMAP_RIGHT(k->Bitmap, k->Depth+1);
	node->key->Depth = k->Depth+1;
	return node->key;
}

template <class T>
Tree::Node::Key *Tree::BinaryReturn<T>::appendLeft(Tree::Node::BinaryReturn<T> *node)
{
	this->current->left = node;
	node->parent = this->Current();
	Tree::Node::Key *k = this->Current()->key;
	node->key->Bitmap = BITMAP_LEFT(k->Bitmap, k->Depth+1);
	node->key->Depth = k->Depth+1;
	return node->key;
}

template <class T>
Tree::Node::Key *Tree::BinaryReturn<T>::appendRight(Tree::BinaryReturn<T> *tree)
{
	Tree::Node::Key *added_key = this->appendRight(tree->getRoot());
	this->setCurrent(tree->getRoot());
	this->updateBelow(false);
	return added_key;
}

template <class T>
Tree::Node::Key *Tree::BinaryReturn<T>::appendLeft(Tree::BinaryReturn<T> *tree)
{
	Tree::Node::Key *added_key = this->appendLeft(tree->getRoot());
	this->setCurrent(tree->getRoot());
	this->updateBelow(true);
	return added_key;
}

template <class T>
List::List<T> *Tree::BinaryReturn<T>::pathTo(Node::Key *key)
{
	// TODO: Sciezka do wezla
}

#endif /* __TREE_H__ */