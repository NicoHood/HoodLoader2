/*
LUFA Library
Copyright (C) Dean Camera, 2014.

dean [at] fourwalledcubicle [dot] com
www.lufa-lib.org
*/

/*
Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

Permission to use, copy, modify, distribute, and sell this
software and its documentation for any purpose is hereby granted
without fee, provided that the above copyright notice appear in
all copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of the author not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

The author disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall the author be liable for any
special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/

/** \file
*  \brief Lightweight ring (circular) buffer, for fast insertion/deletion of bytes.
*
*  Lightweight ring buffer, for fast insertion/deletion. Multiple buffers can be created of
*  different sizes to suit different needs.
*
*  Note that for each buffer, insertion and removal operations may occur at the same time (via
*  a multi-threaded ISR based system) however the same kind of operation (two or more insertions
*  or deletions) must not overlap. If there is possibility of two or more of the same kind of
*  operating occurring at the same point in time, atomic (mutex) locking should be used.
*/

/** \ingroup Group_MiscDrivers
*  \defgroup Group_RingBuff Generic Byte Ring Buffer - LUFA/Drivers/Misc/RingBuffer.h
*  \brief Lightweight ring buffer, for fast insertion/deletion of bytes.
*
*  \section Sec_RingBuff_Dependencies Module Source Dependencies
*  The following files must be built with any user project that uses this module:
*    - None
*
*  \section Sec_RingBuff_ModDescription Module Description
*  Lightweight ring buffer, for fast insertion/deletion. Multiple buffers can be created of
*  different sizes to suit different needs.
*
*  Note that for each buffer, insertion and removal operations may occur at the same time (via
*  a multi-threaded ISR based system) however the same kind of operation (two or more insertions
*  or deletions) must not overlap. If there is possibility of two or more of the same kind of
*  operating occurring at the same point in time, atomic (mutex) locking should be used.
*
*  \section Sec_RingBuff_ExampleUsage Example Usage
*  The following snippet is an example of how this module may be used within a typical
*  application.
*
*  \code
*      // Create the buffer structure and its underlying storage array
*      RingBuffer_t Buffer;
*      uint8_t      BufferData[128];
*
*      // Initialize the buffer with the created storage array
*      RingBuffer_InitBuffer(&Buffer, BufferData, sizeof(BufferData));
*
*      // Insert some data into the buffer
*      RingBuffer_Insert(&Buffer, 'H');
*      RingBuffer_Insert(&Buffer, 'E');
*      RingBuffer_Insert(&Buffer, 'L');
*      RingBuffer_Insert(&Buffer, 'L');
*      RingBuffer_Insert(&Buffer, 'O');
*
*      // Cache the number of stored bytes in the buffer
*      uint16_t BufferCount = RingBuffer_GetCount(&Buffer);
*
*      // Printer stored data length
*      printf("Buffer Length: %d, Buffer Data: \r\n", BufferCount);
*
*      // Print contents of the buffer one character at a time
*      while (BufferCount--)
*        putc(RingBuffer_Remove(&Buffer));
*  \endcode
*
*  @{
*/

#ifndef __LIGHTWEIGHT_RING_BUFFER_H__
#define __LIGHTWEIGHT_RING_BUFFER_H__

/* Includes: */
#include <LUFA/Common/Common.h>

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

	/* Type Defines: */
	/** \brief Ring Buffer Management Structure.
	*
	*  Type define for a new ring buffer object. Buffers should be initialized via a call to
	*  \ref RingBuffer_InitBuffer() before use.
	*/
	typedef struct
	{
		uint8_t* Base; // Pointer to the first address of the buffer
		uint8_t Index; // Current storage location in the circular buffer.
		uint8_t Size;  // Size of the buffer's underlying storage array
		uint8_t Count; // Number of bytes currently stored in the buffer
	} LRingBuffer_t;


	/* Inline Functions: */
	/** Initializes a ring buffer ready for use. Buffers must be initialized via this function
	*  before any operations are called upon them. Already initialized buffers may be reset
	*  by re-initializing them using this function.
	*
	*  \param[out] Buffer   Pointer to a ring buffer structure to initialize.
	*  \param[out] DataPtr  Pointer to a global array that will hold the data stored into the ring buffer.
	*  \param[out] Size     Maximum number of bytes that can be stored in the underlying data array.
	*/
	static inline void LRingBuffer_InitBuffer(LRingBuffer_t* Buffer,
		uint8_t* const DataPtr, const uint8_t Size) ATTR_NON_NULL_PTR_ARG(1) ATTR_NON_NULL_PTR_ARG(2);
	static inline void LRingBuffer_InitBuffer(LRingBuffer_t* Buffer,
		uint8_t* const DataPtr, const uint8_t Size)
	{
		GCC_FORCE_POINTER_ACCESS(Buffer);

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		Buffer->Base = DataPtr;
		Buffer->Index = 0;
		Buffer->Size = Size;
		Buffer->Count = 0;

		SetGlobalInterruptMask(CurrentGlobalInt);
	}

	static inline void LRingBuffer_DisableBuffer(LRingBuffer_t* Buffer) ATTR_NON_NULL_PTR_ARG(1);
	static inline void LRingBuffer_DisableBuffer(LRingBuffer_t* Buffer)
	{
		GCC_FORCE_POINTER_ACCESS(Buffer);

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		Buffer->Base = NULL;
		Buffer->Index = 0;
		Buffer->Size = 0;
		Buffer->Count = 0;

		SetGlobalInterruptMask(CurrentGlobalInt);
	}

	static inline void LRingBuffer_ResetBuffer(LRingBuffer_t* Buffer) ATTR_NON_NULL_PTR_ARG(1);
	static inline void LRingBuffer_ResetBuffer(LRingBuffer_t* Buffer)
	{
		GCC_FORCE_POINTER_ACCESS(Buffer);

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		Buffer->Index = 0;
		Buffer->Count = 0;

		SetGlobalInterruptMask(CurrentGlobalInt);
	}

	static inline bool LRingBuffer_IsEnabled(LRingBuffer_t* const Buffer) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline bool LRingBuffer_IsEnabled(LRingBuffer_t* const Buffer)
	{
		bool state;

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		// check if Base points to a buffer
		if (Buffer->Base == NULL)
			state = false;
		else
			state = true;

		SetGlobalInterruptMask(CurrentGlobalInt);

		return state;
	}

	/** Retrieves the current number of bytes stored in a particular buffer. This value is computed
	*  by entering an atomic lock on the buffer, so that the buffer cannot be modified while the
	*  computation takes place. This value should be cached when reading out the contents of the buffer,
	*  so that as small a time as possible is spent in an atomic lock.
	*
	*  \note The value returned by this function is guaranteed to only be the minimum number of bytes
	*        stored in the given buffer; this value may change as other threads write new data, thus
	*        the returned number should be used only to determine how many successive reads may safely
	*        be performed on the buffer.
	*
	*  \param[in] Buffer  Pointer to a ring buffer structure whose count is to be computed.
	*
	*  \return Number of bytes currently stored in the buffer.
	*/
	static inline uint8_t LRingBuffer_GetCount(LRingBuffer_t* const Buffer) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline uint8_t LRingBuffer_GetCount(LRingBuffer_t* const Buffer)
	{
		uint8_t Count;

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		Count = Buffer->Count;

		SetGlobalInterruptMask(CurrentGlobalInt);
		return Count;
	}

	/** Retrieves the free space in a particular buffer. This value is computed by entering an atomic lock
	*  on the buffer, so that the buffer cannot be modified while the computation takes place.
	*
	*  \note The value returned by this function is guaranteed to only be the maximum number of bytes
	*        free in the given buffer; this value may change as other threads write new data, thus
	*        the returned number should be used only to determine how many successive writes may safely
	*        be performed on the buffer when there is a single writer thread.
	*
	*  \param[in] Buffer  Pointer to a ring buffer structure whose free count is to be computed.
	*
	*  \return Number of free bytes in the buffer.
	*/
	static inline uint8_t LRingBuffer_GetFreeCount(LRingBuffer_t* const Buffer) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline uint8_t LRingBuffer_GetFreeCount(LRingBuffer_t* const Buffer)
	{
		return (Buffer->Size - LRingBuffer_GetCount(Buffer));
	}

	/** Atomically determines if the specified ring buffer contains any data. This should
	*  be tested before removing data from the buffer, to ensure that the buffer does not
	*  underflow.
	*
	*  If the data is to be removed in a loop, store the total number of bytes stored in the
	*  buffer (via a call to the \ref RingBuffer_GetCount() function) in a temporary variable
	*  to reduce the time spent in atomicity locks.
	*
	*  \param[in,out] Buffer  Pointer to a ring buffer structure to insert into.
	*
	*  \return Boolean \c true if the buffer contains no free space, \c false otherwise.
	*/
	static inline bool LRingBuffer_IsEmpty(LRingBuffer_t* const Buffer) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline bool LRingBuffer_IsEmpty(LRingBuffer_t* const Buffer)
	{
		return (LRingBuffer_GetCount(Buffer) == 0);
	}

	/** Atomically determines if the specified ring buffer contains any free space. This should
	*  be tested before storing data to the buffer, to ensure that no data is lost due to a
	*  buffer overrun.
	*
	*  \param[in,out] Buffer  Pointer to a ring buffer structure to insert into.
	*
	*  \return Boolean \c true if the buffer contains no free space, \c false otherwise.
	*/
	static inline bool LRingBuffer_IsFull(LRingBuffer_t* const Buffer) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline bool LRingBuffer_IsFull(LRingBuffer_t* const Buffer)
	{
		return (LRingBuffer_GetCount(Buffer) == Buffer->Size);
	}


	/** Inserts an element into the ring buffer.
	*
	*  \warning Only one execution thread (main program thread or an ISR) may insert into a single buffer
	*           otherwise data corruption may occur. Insertion and removal may occur from different execution
	*           threads.
	*
	*  \param[in,out] Buffer  Pointer to a ring buffer structure to insert into.
	*  \param[in]     Data    Data element to insert into the buffer.
	*/
	static inline void LRingBuffer_Insert(LRingBuffer_t* Buffer, const uint8_t Data) ATTR_NON_NULL_PTR_ARG(1);
	static inline void LRingBuffer_Insert(LRingBuffer_t* Buffer, const uint8_t Data)
	{
		GCC_FORCE_POINTER_ACCESS(Buffer);

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		// check if buffer is disabled or already full
		if ((Buffer->Count == Buffer->Size)){
			SetGlobalInterruptMask(CurrentGlobalInt);
			return;
		}

		// save new data to the current index and return to its beginning if it reaches the bound
		Buffer->Base[Buffer->Index] = Data;
		//TODO
		//Buffer->Index = (Buffer->Index + 1) % Buffer->Size;
		if (Buffer->Index == Buffer->Size)
			Buffer->Index = 0;

		Buffer->Count++;

		SetGlobalInterruptMask(CurrentGlobalInt);
	}

	static inline void LRingBuffer_Append(LRingBuffer_t* Buffer, const uint8_t Data) ATTR_NON_NULL_PTR_ARG(1);
	static inline void LRingBuffer_Append(LRingBuffer_t* Buffer, const uint8_t Data)
	{
		GCC_FORCE_POINTER_ACCESS(Buffer);

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		// check if buffer is disabled
		if ((!Buffer->Base)){
			SetGlobalInterruptMask(CurrentGlobalInt);
			return;
		}

		// save new data before the first item
		int pos = Buffer->Index - Buffer->Count - 1;
		if (pos < 0)
			pos = Buffer->Size + pos;
		Buffer->Base[pos] = Data;

		// correct index position if its full 
		if (Buffer->Count == Buffer->Size){
			if (Buffer->Index == 0)
				Buffer->Index = Buffer->Size;
			Buffer->Index--;
		}
		// increase Counter if its not full. This will overwrite newer bytes if its already full!
		else Buffer->Count++;

		SetGlobalInterruptMask(CurrentGlobalInt);
	}

	static inline void LRingBuffer_Append_Buffer(LRingBuffer_t* Buffer,
		uint8_t* const DataPtr, const uint8_t Size) ATTR_NON_NULL_PTR_ARG(1) ATTR_NON_NULL_PTR_ARG(2);
	static inline void LRingBuffer_Append_Buffer(LRingBuffer_t* Buffer,
		uint8_t* const DataPtr, const uint8_t Size)
	{
		// append all bytes, last byte first
		for (int i = 0; i < Size; i++)
			LRingBuffer_Append(Buffer, DataPtr[Size - i - 1]);
	}

	/** Removes an element from the ring buffer.
	*
	*  \warning Only one execution thread (main program thread or an ISR) may remove from a single buffer
	*           otherwise data corruption may occur. Insertion and removal may occur from different execution
	*           threads.
	*
	*  \param[in,out] Buffer  Pointer to a ring buffer structure to retrieve from.
	*
	*  \return Next data element stored in the buffer.
	*/
	static inline int LRingBuffer_Remove(LRingBuffer_t* Buffer) ATTR_NON_NULL_PTR_ARG(1);
	static inline int LRingBuffer_Remove(LRingBuffer_t* Buffer)
	{
		GCC_FORCE_POINTER_ACCESS(Buffer);

		uint8_t Data;

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		// check if buffer is disabled or empty
		if ((Buffer->Base == NULL) || (Buffer->Count == 0)){
			SetGlobalInterruptMask(CurrentGlobalInt);
			return -1;
		}

		// get last item position
		int pos = Buffer->Index - Buffer->Count;
		if (pos < 0)
			pos = Buffer->Size + pos;

		Data = Buffer->Base[pos];
		Buffer->Count--;

		SetGlobalInterruptMask(CurrentGlobalInt);

		return Data;
	}

	static inline int LRingBuffer_Peek_Pos(LRingBuffer_t* Buffer, const uint8_t position) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline int LRingBuffer_Peek_Pos(LRingBuffer_t* Buffer, const uint8_t position)
	{
		uint8_t Data;

		uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
		GlobalInterruptDisable();

		// check if buffer is disabled, empty, or not full enough
		if ((Buffer->Base == NULL) || (Buffer->Count == 0) || position > (Buffer->Count - 1)){
			SetGlobalInterruptMask(CurrentGlobalInt);
			return -1;
		}

		// get last item position
		int pos = Buffer->Index - Buffer->Count + position;
		if (pos < 0)
			pos = Buffer->Size + pos;

		Data = Buffer->Base[pos];

		SetGlobalInterruptMask(CurrentGlobalInt);

		return Data;
	}

	/** Returns the next element stored in the ring buffer, without removing it.
	*
	*  \param[in,out] Buffer  Pointer to a ring buffer structure to retrieve from.
	*
	*  \return Next data element stored in the buffer.
	*/
	static inline int LRingBuffer_Peek(LRingBuffer_t* Buffer) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(1);
	static inline int LRingBuffer_Peek(LRingBuffer_t* Buffer)
	{
		return LRingBuffer_Peek_Pos(Buffer, 0);
	}

	/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif

/** @} */

