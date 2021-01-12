/**
 * @file list.h
 *
 * @date Jun 17, 2010
 * @author stefan
 * @brief A template class for static (fixed size) arrays
 */

#ifndef STATICLIST_H_
#define STATICLIST_H_

#include <cpp/exception.h>

class IndexOutOfBoundsExecption : public RayKernel::Exception
{
public:
	IndexOutOfBoundsExecption();
};

template <typename T>
class StaticList
{
public:

	/// readonly variable
	UINT32& Size;

	/**
	 * Constructor for an empty list
	 * @param numItems Number of empty items to create
	 */
	StaticList<T>(UINT32 numItems)
	: Size(_numItems),
	  _numItems(numItems),
	  _createdPayload(true)
	{
		if (numItems)
		{
			this->_payload = new T[numItems];
		}
		else
		{
			this->_payload = NULL;
		}
	}

	/**
	 * Constructor out of an array of numItems of type T
	 */
	StaticList<T>(T* original, UINT32 numItems)
	: Size(_numItems),
	  _numItems(numItems),
	  _payload(original),
	  _createdPayload(false)
	{

	}

	/**
	 * Copy constructor: List parts
	 */
	StaticList<T>(StaticList<T>& original, UINT32 fromIndex, UINT32 length)
	: Size(_numItems),
	  _numItems(length),
	  _payload(original._payload + fromIndex),
	  _createdPayload(false)
	{

	}

	StaticList<T>(StaticList<T>& original)
	: Size(_numItems),
	  _numItems(original._numItems),
	  _payload(original._payload),
	  _createdPayload(false)
	{

	}

	virtual ~StaticList<T>()
	{
		if (_createdPayload)
		{
			delete[] _payload;
		}
	}

	/**
	 * Provides the standard index operator []
	 * to access the list like:
	 * StaticList[42]
	 * @param index Index into the list
	 * @return Pointer to the list index
	 */
	virtual T& operator [] (unsigned index) throw (IndexOutOfBoundsExecption)
	{
		if (index < this->_numItems)
		{
			return this->_payload[index];
		}
		else
		{
			throw new IndexOutOfBoundsExecption();
		}
	}

	/**
	 * Sets the given index to the contents specified in source
	 * @param index Index to set
	 * @param source Source pointer (will be copied! to [index])
	 */
	virtual void Set(UINT32 index, T* source) throw (IndexOutOfBoundsExecption)
	{
		if (index < this->_numItems)
		{
			memcpy(this->_payload + index, source, sizeof(T));
		}
		else
		{
			throw new IndexOutOfBoundsExecption();
		}
	}

	/**
	 * Truncates the current list. The list will be reduced to a size
	 * of index, meaning that the given index will NOT be part of this
	 * list anymore.
	 * @param index
	 */
	virtual void TruncateAtIndex(UINT32 index)
	{
		if (index == 0)
		{
			// an empty list would remain
			throw new IndexOutOfBoundsExecption();
		}

		if ((index > 0) && (index < this->_numItems))
		{
			this->_numItems = index;
		}
		else
		{
			throw new IndexOutOfBoundsExecption();
		}
	}

	/**
	 * Returns the index of a pointer, that resides in the list.
	 */
	virtual UINT32 GetIndexOf(T* search)
	{
		SINT32 absoluteDistance = (UINT32)search - (UINT32)&_payload[0];

		// cannot be negative
		if (absoluteDistance < 0)
		{
			throw new IndexOutOfBoundsExecption();
		}

		// and must be a real index
		if (absoluteDistance % sizeof(T) == 0)
		{
			UINT32 index = search - &_payload[0];
			if (index < _numItems)
			{
				return index;
			}
			else
			{
				throw new IndexOutOfBoundsExecption();
			}
		}
		else
		{
			throw new IndexOutOfBoundsExecption();
		}
	}

	POINTER GetPayloadPointer()
	{
		return static_cast<POINTER>(this->_payload);
	}

	UINT32 GetPayloadByteLength()
	{
		return sizeof(T) * this->_numItems;
	}

protected:
	UINT32 _numItems;
	T* _payload;

	bool _createdPayload;
};

template <typename T>
class SortedList : public StaticList<T>
{
public:

	typedef bool (*SortBiggerFunction)(T& p1, T& p2);

	SortedList<T>(UINT32 size)
	: StaticList<T>(size)
	{
		InitIndirections();
	}

	SortedList<T>(StaticList<T>& base)
	: StaticList<T>(base)
	{
		InitIndirections();
	}

	void InitIndirections()
	{
		_indirection = new UINT32[this->_numItems];
		for(UINT32 i = 0; i < this->_numItems; i++)
		{
			_indirection[i] = i;
		}
	}


	/**
	 * Sort using bubble sort
	 */
	void Sort(SortBiggerFunction sortFunc)
	{
		SINT32 n = this->_numItems;
		bool touched = false;
		do
		{
			touched = false;
			for(SINT32 i = 0; i <= n - 2; i++)
			{
				//if (this->_payload[i] > this->_payload[i+1])
				if (sortFunc(this->_payload[_indirection[i]], this->_payload[_indirection[i+1]]))
				{
					// swap them
					UINT32 temp = _indirection[i];
					_indirection[i] = _indirection[i+1];
					_indirection[i+1] = temp;
					touched = true;
				}
			}
			n--;
		}while((touched) && (n > 1));
	}

	/**
	 * Provides the standard index operator []
	 * to access the list like:
	 * StaticList[42]
	 * @param index Index into the list
	 * @return Pointer to the list index
	 */
	virtual T& operator [] (unsigned index) throw (IndexOutOfBoundsExecption)
	{
		return StaticList<T>::operator [](_indirection[index]);
	}

	/**
	 * Sets the given index to the contents specified in source
	 * @param index Index to set
	 * @param source Source pointer (will be copied! to [index])
	 */
	virtual void Set(UINT32 index, T* source) throw (IndexOutOfBoundsExecption)
	{
		StaticList<T>::Set(_indirection[index], source);
	}

	/**
	 * Truncating is not defined for sorted lists, as it would not be
	 * clear whether to truncate at the index visible from the outside
	 * (thus, using the indirection)
	 * @param index
	 */
	virtual void TruncateAtIndex(UNUSED UINT32 index)
	{
		// Behaviour of sorted truncating cannot be defined!
		throw new IndexOutOfBoundsExecption();
	}

	virtual UINT32 GetIndexOf(UNUSED T* search)
	{
		// Behaviour of sorted truncating cannot be defined!
		throw new IndexOutOfBoundsExecption();
	}

private:
	UINT32* _indirection;

};

#endif /* STATICLIST_H_ */
