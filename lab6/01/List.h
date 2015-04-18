/*
 * Metody Programowania - Laboratorium 6 - Program 01
 * Implementacja klas: lista
 * Grzegorz Kowalski, 12i
 * wersja 2 | 04.2015
 */

#include <stdio.h>

#ifndef __LIST_H__
#define __LIST_H__

namespace List
{
	template <class T>
	class List;

	// Definicje
	template <class T>
	class Element
	{
		friend class List<T>;
	private:
		T data;
		Element<T> *p_next;

	public:
		Element(T data);
		~Element();
		Element<T> *next(void);
		T *getData(void);
		void remove(void);
	};

	template <class T>
	class List
	{
	private:
		Element<T> *root;
		int len;
		Element<T> *cur;

	public:
		List();
		~List();
		Element<T> *get(int i);
		int getLen(void);
		int append(Element<T> *e);
		int prepend(Element<T> *e);
		int appendAfter(Element<T> *e, int i);
		int prependBefore(Element<T> *e, int i);
		int remove(int i);
		void reset(void);
		Element<T> *current(void);
		Element<T> *next(void);
		int operator+=(Element<T> *e);
		Element<T> *operator[](int i);
		operator bool();
		bool operator!();
	};

	// Implementacja

	// ----- class Element
	template <class T>
	Element<T>::Element(T data)
	{
		this->data = data;
		this->p_next = NULL;
	}

	template <class T>
	Element<T>::~Element()
	{
	}

	template <class T>
	Element<T> *Element<T>::next(void)
	{
		return this->p_next;
	}

	template <class T>
	T *Element<T>::getData(void)
	{
		return &this->data;	// zwracam wskaznik do danych, bo dane moga byc typu zlozonego
	}

	// ----- class List
	template <class T>
	List<T>::List()
	{
		this->root = NULL;
		this->len = 0;
	}

	template <class T>
	List<T>::~List()
	{
	}

	template <class T>
	Element<T> *List<T>::get(int i)
	{
		if(i <= this->len && i > 0)
		{
			Element<T> *e = this->root;
			for(int j=1; j<i; j++) e = e->next();	// przejdz do i-tego elementu listy
			return e; 
		}
		return NULL;	// jesli i jest wieksze niz dlugosc listy (lub ujemne) zwroc pusty wskaznik
	}

	template <class T>
	int List<T>::getLen(void)
	{
		return this->len;
	}

	template <class T>
	int List<T>::append(Element<T> *e)
	{
		if(this->len == 0)
			this->root = e;		// jesli lista jest pusta nowy element staje sie glowa
		else
			this->get(this->len)->p_next = e;	// jesli nie, dolaczany jest do ostatniego elementu
		return ++this->len;
	}

	template <class T>
	int List<T>::prepend(Element<T> *e)
	{
		e->p_next = this->root;		// lista dolaczana jest do nowego elementu
		this->root = e;				// nowy elelment staje sie glowa
		return ++this->len;
	}

	template <class T>
	int List<T>::appendAfter(Element<T> *e, int i)
	{
		Element<T> *after;
		if((after = this->get(i)) == NULL) return -1;	// jesli element za ktory wstawiam nie istnieje, zwroc blad
		e->p_next = after->next();		// nowy element wskazuje na nastepnik poprzedniego
		after->p_next = e;			// poprzedni wskazuje na nowy
		return ++this->len;
	}

	template <class T>
	int List<T>::prependBefore(Element<T> *e, int i)
	{
		return this->appendAfter(e, i-1);	// dodanie przed i-ty element mozna sprowadzic do dodania za (i-1)
	}

	template <class T>
	int List<T>::remove(int i)
	{
		if(i <= this->len && i > 0)
		{
			Element<T> *prev = this->get(i-1);
			Element<T> *current = prev->next();
			prev->p_next = current->next();		// element poprzedni dostaje adres nastepnego
			delete current;		// aktualny jest usuwany
			return --this->len;
		}
		return -1;	// element nie istnieje
	}

	template <class T>
	void List<T>::reset(void)
	{
		this->cur = this->root;
	}

	template <class T>
	Element<T> *List<T>::current(void)
	{
		return this->cur;
	}

	template <class T>
	Element<T> *List<T>::next(void)
	{
		this->cur = this->cur->next();
		return this->current();
	}

	template <class T>
	int List<T>::operator+=(Element<T> *e)
	{
		return this->append(e);
	}

	template <class T>
	Element<T> *List<T>::operator[](int i)
	{
		return this->get(i);
	}

	template <class T>
	List<T>::operator bool()
	{
		return (this->len > 0);
	}

	template <class T>
	bool List<T>::operator!()
	{
		return (this->len <= 0);
	}
}

#endif /* __LIST_H__ */