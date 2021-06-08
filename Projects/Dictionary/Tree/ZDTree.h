#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <set>
#include <queue>
#include <algorithm>
#include <cstddef>

namespace Dico
{
	/// A node in the ZDTree, combining links to other nodes as well as the actual data.
	template<class T>
	class TreeNode 
	{ 
		// size: 5*4=20 bytes (on 32 bit arch), can be reduced by 8.
		public:
		TreeNode();
		TreeNode(const T&);
		TreeNode(T&&);

		TreeNode<T>* parent;
		TreeNode<T>* first_child, * last_child;
		TreeNode<T>* prev_sibling, * next_sibling;
		T data;
	};

	template<class T>
	TreeNode<T>::TreeNode()
		: parent(0), first_child(0), last_child(0), prev_sibling(0), next_sibling(0)
	{
	}

	template<class T>
	TreeNode<T>::TreeNode(const T& val)
		: parent(0), first_child(0), last_child(0), prev_sibling(0), next_sibling(0), data(val)
	{
	}

	template<class T>
	TreeNode<T>::TreeNode(T&& val)
		: parent(0), first_child(0), last_child(0), prev_sibling(0), next_sibling(0), data(val)
	{
	}


	template <class T, class Tree_node_allocator = std::allocator<TreeNode<T> > >
	class ZDTree 
	{
		protected:
			typedef TreeNode<T> Tree_node;
		public:
			/// Value of the data stored at a node.
			typedef T value_type;

			class iterator_base;
			class pre_order_iterator;
			class sibling_iterator;

			ZDTree();                                    
			~ZDTree();
		
			/// Erase all nodes of the ZDTree.
			void     clear();
			/// Erase element at position pointed to by iterator, return incremented iterator.
			template<typename iter> iter erase(iter);
			/// Erase all children of the node pointed to by iterator.
			void     erase_children(const iterator_base&);

			/// Return iterator to the beginning of the ZDTree.
			inline pre_order_iterator   begin() const;
			/// Return iterator to the end of the ZDTree.
			inline pre_order_iterator   end() const;

			/// Return sibling iterator to the first child of given node.
			static sibling_iterator     begin(const iterator_base&);
			/// Return sibling end iterator for children of given node.
			static sibling_iterator     end(const iterator_base&);

			/// Insert node as previous sibling of node pointed to by position.
			template<typename iter> iter insert(iter position, const T& x);
			template<typename iter> iter insert(iter position, T&& x);

			/// Insert empty node as last/first child of node pointed to by position.
			template<typename iter> iter append_child(iter position);

			/// Insert node as last/first child of node pointed to by position.
			template<typename iter> iter append_child(iter position, const T& x);
			template<typename iter> iter append_child(iter position, T&& x);


		  /// Base class for iterators, only pointers stored, no traversal logic.
		  /// 
			class iterator_base 
			{
				public:
					typedef T                               value_type;
					typedef T* pointer;
					typedef T& reference;
					typedef size_t                          size_type;
					typedef ptrdiff_t                       difference_type;
					typedef std::bidirectional_iterator_tag iterator_category;

					iterator_base();
					iterator_base(Tree_node*);

					T& operator*() const;
					T* operator->() const;

					/// When called, the next increment/decrement skips children of this node.
					void         skip_children();
					void         skip_children(bool skip);
					/// Number of children of the node pointed to by the iterator.
					unsigned int number_of_children() const;

					sibling_iterator begin() const;
					sibling_iterator end() const;

					Tree_node* node;
				protected:
					bool skip_current_children_;
			};

			/// Depth-first iterator, first accessing the node, then its children.
			class pre_order_iterator : public iterator_base 
			{
				public:
					pre_order_iterator();
					pre_order_iterator(Tree_node*);
					pre_order_iterator(const iterator_base&);
					pre_order_iterator(const sibling_iterator&);

					bool    operator==(const pre_order_iterator&) const;
					bool    operator!=(const pre_order_iterator&) const;
					pre_order_iterator& operator++();
					pre_order_iterator& operator--();
					pre_order_iterator   operator++(int);
					pre_order_iterator   operator--(int);
					pre_order_iterator& operator+=(unsigned int);
					pre_order_iterator& operator-=(unsigned int);

					pre_order_iterator& next_skip_children();
			};

			
			/// The default iterator types throughout the ZDTree class.
			typedef pre_order_iterator            iterator;

			/// Iterator which traverses only the nodes which are siblings of each other.
			class sibling_iterator : public iterator_base 
			{
				public:
					sibling_iterator();
					sibling_iterator(Tree_node*);
					sibling_iterator(const sibling_iterator&);
					sibling_iterator(const iterator_base&);

					bool    operator==(const sibling_iterator&) const;
					bool    operator!=(const sibling_iterator&) const;
					sibling_iterator& operator++();
					sibling_iterator& operator--();
					sibling_iterator   operator++(int);
					sibling_iterator   operator--(int);
					sibling_iterator& operator+=(unsigned int);
					sibling_iterator& operator-=(unsigned int);

					Tree_node* range_first() const;
					Tree_node* range_last() const;
					Tree_node* parent_;
				private:
					void set_parent_();
			};

			Tree_node* head, * feet;    // head/feet are always dummy; if an iterator points to them it is invalid

	private :
		Tree_node_allocator m_alloc;
		void head_initialise();
	};


	// Iterator base

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::iterator_base::iterator_base()
		: node(0), skip_current_children_(false)
	{
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::iterator_base::iterator_base(Tree_node* tn)
		: node(tn), skip_current_children_(false)
	{
	}

	template <class T, class Tree_node_allocator>
	T& ZDTree<T, Tree_node_allocator>::iterator_base::operator*() const
	{
		return node->data;
	}

	template <class T, class Tree_node_allocator>
	T* ZDTree<T, Tree_node_allocator>::iterator_base::operator->() const
	{
		return &(node->data);
	}


	template <class T, class Tree_node_allocator>
	bool ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator!=(const pre_order_iterator& other) const
	{
		if (other.node != this->node) return true;
		else return false;
	}

	template <class T, class Tree_node_allocator>
	bool ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator==(const pre_order_iterator& other) const
	{
		if (other.node == this->node) return true;
		else return false;
	}

	template <class T, class Tree_node_allocator>
	bool ZDTree<T, Tree_node_allocator>::sibling_iterator::operator!=(const sibling_iterator& other) const
	{
		if (other.node != this->node) return true;
		else return false;
	}

	template <class T, class Tree_node_allocator>
	bool ZDTree<T, Tree_node_allocator>::sibling_iterator::operator==(const sibling_iterator& other) const
	{
		if (other.node == this->node) return true;
		else return false;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator ZDTree<T, Tree_node_allocator>::iterator_base::begin() const
	{
		if (node->first_child == 0)
			return end();

		sibling_iterator ret(node->first_child);
		ret.parent_ = this->node;
		return ret;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator ZDTree<T, Tree_node_allocator>::iterator_base::end() const
	{
		sibling_iterator ret(0);
		ret.parent_ = node;
		return ret;
	}

	template <class T, class Tree_node_allocator>
	void ZDTree<T, Tree_node_allocator>::iterator_base::skip_children()
	{
		skip_current_children_ = true;
	}

	template <class T, class Tree_node_allocator>
	void ZDTree<T, Tree_node_allocator>::iterator_base::skip_children(bool skip)
	{
		skip_current_children_ = skip;
	}

	template <class T, class Tree_node_allocator>
	unsigned int ZDTree<T, Tree_node_allocator>::iterator_base::number_of_children() const
	{
		Tree_node* pos = node->first_child;
		if (pos == 0) return 0;

		unsigned int ret = 1;
		while (pos != node->last_child) {
			++ret;
			pos = pos->next_sibling;
		}
		return ret;
	}



	// Pre-order iterator

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::pre_order_iterator::pre_order_iterator()
		: iterator_base(0)
	{
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::pre_order_iterator::pre_order_iterator(Tree_node* tn)
		: iterator_base(tn)
	{
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::pre_order_iterator::pre_order_iterator(const iterator_base& other)
		: iterator_base(other.node)
	{
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::pre_order_iterator::pre_order_iterator(const sibling_iterator& other)
		: iterator_base(other.node)
	{
		if (this->node == 0) {
			if (other.range_last() != 0)
				this->node = other.range_last();
			else
				this->node = other.parent_;
			this->skip_children();
			++(*this);
		}
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator& ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator++()
	{
		assert(this->node != 0);
		if (!this->skip_current_children_ && this->node->first_child != 0) {
			this->node = this->node->first_child;
		}
		else {
			this->skip_current_children_ = false;
			while (this->node->next_sibling == 0) {
				this->node = this->node->parent;
				if (this->node == 0)
					return *this;
			}
			this->node = this->node->next_sibling;
		}
		return *this;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator& ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator--()
	{
		assert(this->node != 0);
		if (this->node->prev_sibling) {
			this->node = this->node->prev_sibling;
			while (this->node->last_child)
				this->node = this->node->last_child;
		}
		else {
			this->node = this->node->parent;
			if (this->node == 0)
				return *this;
		}
		return *this;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator++(int)
	{
		pre_order_iterator copy = *this;
		++(*this);
		return copy;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator& ZDTree<T, Tree_node_allocator>::pre_order_iterator::next_skip_children()
	{
		(*this).skip_children();
		(*this)++;
		return *this;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator--(int)
	{
		pre_order_iterator copy = *this;
		--(*this);
		return copy;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator& ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator+=(unsigned int num)
	{
		while (num > 0) {
			++(*this);
			--num;
		}
		return (*this);
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator& ZDTree<T, Tree_node_allocator>::pre_order_iterator::operator-=(unsigned int num)
	{
		while (num > 0) {
			--(*this);
			--num;
		}
		return (*this);
	}

	// Sibling iterator

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::sibling_iterator::sibling_iterator()
		: iterator_base()
	{
		set_parent_();
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::sibling_iterator::sibling_iterator(Tree_node* tn)
		: iterator_base(tn)
	{
		set_parent_();
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::sibling_iterator::sibling_iterator(const iterator_base& other)
		: iterator_base(other.node)
	{
		set_parent_();
	}

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::sibling_iterator::sibling_iterator(const sibling_iterator& other)
		: iterator_base(other), parent_(other.parent_)
	{
	}

	template <class T, class Tree_node_allocator>
	void ZDTree<T, Tree_node_allocator>::sibling_iterator::set_parent_()
	{
		parent_ = 0;
		if (this->node == 0) return;
		if (this->node->parent != 0)
			parent_ = this->node->parent;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator& ZDTree<T, Tree_node_allocator>::sibling_iterator::operator++()
	{
		if (this->node)
			this->node = this->node->next_sibling;
		return *this;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator& ZDTree<T, Tree_node_allocator>::sibling_iterator::operator--()
	{
		if (this->node) this->node = this->node->prev_sibling;
		else {
			assert(parent_);
			this->node = parent_->last_child;
		}
		return *this;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator ZDTree<T, Tree_node_allocator>::sibling_iterator::operator++(int)
	{
		sibling_iterator copy = *this;
		++(*this);
		return copy;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator ZDTree<T, Tree_node_allocator>::sibling_iterator::operator--(int)
	{
		sibling_iterator copy = *this;
		--(*this);
		return copy;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator& ZDTree<T, Tree_node_allocator>::sibling_iterator::operator+=(unsigned int num)
	{
		while (num > 0) {
			++(*this);
			--num;
		}
		return (*this);
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::sibling_iterator& ZDTree<T, Tree_node_allocator>::sibling_iterator::operator-=(unsigned int num)
	{
		while (num > 0) {
			--(*this);
			--num;
		}
		return (*this);
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::Tree_node* ZDTree<T, Tree_node_allocator>::sibling_iterator::range_first() const
	{
		Tree_node* tmp = parent_->first_child;
		return tmp;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::Tree_node* ZDTree<T, Tree_node_allocator>::sibling_iterator::range_last() const
	{
		return parent_->last_child;
	}

	//ZDTree

	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::ZDTree()
	{
		head_initialise();
	}


	template <class T, class Tree_node_allocator>
	ZDTree<T, Tree_node_allocator>::~ZDTree()
	{
		clear();
		std::allocator_traits<decltype(m_alloc)>::destroy(m_alloc, head);
		std::allocator_traits<decltype(m_alloc)>::destroy(m_alloc, feet);
		std::allocator_traits<decltype(m_alloc)>::deallocate(m_alloc, head, 1);
		std::allocator_traits<decltype(m_alloc)>::deallocate(m_alloc, feet, 1);
	}

	template <class T, class Tree_node_allocator>
	void ZDTree<T, Tree_node_allocator>::clear()
	{
		if (head)
			while (head->next_sibling != feet)
				erase(pre_order_iterator(head->next_sibling));
	}

	
	template<class T, class tree_node_allocator>
	template<class iter>
	iter ZDTree<T, tree_node_allocator>::erase(iter it)
	{
		Tree_node* cur = it.node;
		assert(cur != head);
		iter ret = it;
		ret.skip_children();
		++ret;
		erase_children(it);
		if (cur->prev_sibling == 0) {
			cur->parent->first_child = cur->next_sibling;
		}
		else {
			cur->prev_sibling->next_sibling = cur->next_sibling;
		}
		if (cur->next_sibling == 0) {
			cur->parent->last_child = cur->prev_sibling;
		}
		else {
			cur->next_sibling->prev_sibling = cur->prev_sibling;
		}

		std::allocator_traits<decltype(m_alloc)>::destroy(m_alloc, cur);
		std::allocator_traits<decltype(m_alloc)>::deallocate(m_alloc, cur, 1);
		return ret;
	}

	template<class T, class tree_node_allocator>
	void ZDTree<T, tree_node_allocator>::erase_children(const iterator_base& it)
	{
		//	std::cout << "erase_children " << it.node << std::endl;
		if (it.node == 0) return;

		Tree_node* cur = it.node->first_child;
		Tree_node* prev = 0;

		while (cur != 0) {
			prev = cur;
			cur = cur->next_sibling;
			erase_children(pre_order_iterator(prev));
			std::allocator_traits<decltype(m_alloc)>::destroy(m_alloc, prev);
			std::allocator_traits<decltype(m_alloc)>::deallocate(m_alloc, prev, 1);
		}
		it.node->first_child = 0;
		it.node->last_child = 0;
		//	std::cout << "exit" << std::endl;
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator ZDTree<T, Tree_node_allocator>::begin() const
	{
		return pre_order_iterator(head->next_sibling);
	}

	template <class T, class Tree_node_allocator>
	typename ZDTree<T, Tree_node_allocator>::pre_order_iterator ZDTree<T, Tree_node_allocator>::end() const
	{
		return pre_order_iterator(feet);
	}


	template <class T, class tree_node_allocator>
	typename ZDTree<T, tree_node_allocator>::sibling_iterator ZDTree<T, tree_node_allocator>::begin(const iterator_base& pos)
	{
		assert(pos.node != 0);
		if (pos.node->first_child == 0) {
			return end(pos);
		}
		return pos.node->first_child;
	}

	template <class T, class tree_node_allocator>
	typename ZDTree<T, tree_node_allocator>::sibling_iterator ZDTree<T, tree_node_allocator>::end(const iterator_base& pos)
	{
		sibling_iterator ret(0);
		ret.parent_ = pos.node;
		return ret;
	}


	template <class T, class Tree_node_allocator>
	template <class iter>
	iter ZDTree<T, Tree_node_allocator>::insert(iter position, const T& x)
	{
		if (position.node == 0) {
			position.node = feet; // Backward compatibility: when calling insert on a null node,
								// insert before the feet.
		}
		assert(position.node != head); // Cannot insert before head.

		Tree_node* tmp = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, tmp, x);
		tmp->first_child = 0;
		tmp->last_child = 0;

		tmp->parent = position.node->parent;
		tmp->next_sibling = position.node;
		tmp->prev_sibling = position.node->prev_sibling;
		position.node->prev_sibling = tmp;

		if (tmp->prev_sibling == 0) {
			if (tmp->parent) // when inserting nodes at the head, there is no parent
				tmp->parent->first_child = tmp;
		}
		else
			tmp->prev_sibling->next_sibling = tmp;
		return tmp;
	}

	template <class T, class Tree_node_allocator>
	template <class iter>
	iter ZDTree<T, Tree_node_allocator>::insert(iter position, T&& x)
	{
		if (position.node == 0) {
			position.node = feet; // Backward compatibility: when calling insert on a null node,
								// insert before the feet.
		}
		Tree_node* tmp = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, tmp);
		std::swap(tmp->data, x); // Move semantics
		tmp->first_child = 0;
		tmp->last_child = 0;

		tmp->parent = position.node->parent;
		tmp->next_sibling = position.node;
		tmp->prev_sibling = position.node->prev_sibling;
		position.node->prev_sibling = tmp;

		if (tmp->prev_sibling == 0) {
			if (tmp->parent) // when inserting nodes at the head, there is no parent
				tmp->parent->first_child = tmp;
		}
		else
			tmp->prev_sibling->next_sibling = tmp;
		return tmp;
	}


	template <class T, class tree_node_allocator>
	template <typename iter>
	iter ZDTree<T, tree_node_allocator>::append_child(iter position)
	{
		assert(position.node != head);
		assert(position.node != feet);
		assert(position.node);

		Tree_node* tmp = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, tmp, tree_node_<T>());
		tmp->first_child = 0;
		tmp->last_child = 0;

		tmp->parent = position.node;
		if (position.node->last_child != 0) {
			position.node->last_child->next_sibling = tmp;
		}
		else {
			position.node->first_child = tmp;
		}
		tmp->prev_sibling = position.node->last_child;
		position.node->last_child = tmp;
		tmp->next_sibling = 0;
		return tmp;
	}

	template <class T, class tree_node_allocator>
	template <class iter>
	iter ZDTree<T, tree_node_allocator>::append_child(iter position, const T& x)
	{
		// If your program fails here you probably used 'append_child' to add the top
		// node to an empty ZDTree. From version 1.45 the top element should be added
		// using 'insert'. See the documentation for further information, and sorry about
		// the API change.
		assert(position.node != head);
		assert(position.node != feet);
		assert(position.node);

		Tree_node* tmp = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, tmp, x);
		tmp->first_child = 0;
		tmp->last_child = 0;

		tmp->parent = position.node;
		if (position.node->last_child != 0) {
			position.node->last_child->next_sibling = tmp;
		}
		else {
			position.node->first_child = tmp;
		}
		tmp->prev_sibling = position.node->last_child;
		position.node->last_child = tmp;
		tmp->next_sibling = 0;
		return tmp;
	}

	template <class T, class tree_node_allocator>
	template <class iter>
	iter ZDTree<T, tree_node_allocator>::append_child(iter position, T&& x)
	{
		assert(position.node != head);
		assert(position.node != feet);
		assert(position.node);

		Tree_node* tmp = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, tmp); // Here is where the move semantics kick in
		std::swap(tmp->data, x);

		tmp->first_child = 0;
		tmp->last_child = 0;

		tmp->parent = position.node;
		if (position.node->last_child != 0) {
			position.node->last_child->next_sibling = tmp;
		}
		else {
			position.node->first_child = tmp;
		}
		tmp->prev_sibling = position.node->last_child;
		position.node->last_child = tmp;
		tmp->next_sibling = 0;
		return tmp;
	}


	template <class T, class Tree_node_allocator>
	void ZDTree<T, Tree_node_allocator>::head_initialise()
	{
		head = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		feet = std::allocator_traits<decltype(m_alloc)>::allocate(m_alloc, 1, 0);
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, head, TreeNode<T>());
		std::allocator_traits<decltype(m_alloc)>::construct(m_alloc, feet, TreeNode<T>());

		head->parent = 0;
		head->first_child = 0;
		head->last_child = 0;
		head->prev_sibling = 0; //head;
		head->next_sibling = feet; //head;

		feet->parent = 0;
		feet->first_child = 0;
		feet->last_child = 0;
		feet->prev_sibling = head;
		feet->next_sibling = 0;
	}
}