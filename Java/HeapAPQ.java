package cs2321;

import java.util.Comparator;
import net.datastructures.*;

/**
 * An Adaptable PriorityQueue based on a heap
 * 
 * Course: CS2321 Section ALL
 * Assignment: #3
 * @author - Ryan Cole
 */
public class HeapAPQ<K, V> implements AdaptablePriorityQueue<K, V> {

	public static class DefaultComparator<K> implements Comparator<K> {
		
		// This compare method simply calls the compareTo method of the argument. 
		// If the argument is not a Comparable object, and therefore there is no compareTo method,
		// it will throw ClassCastException. 
		public int compare( K a, K b ) throws IllegalArgumentException {
			if ( a instanceof Comparable )
			   return ( (Comparable <K>) a ).compareTo( b );
			else
				throw new IllegalArgumentException();
		}
	}

	/**
	 * Inner class which modifies Entry<K, V> so each entry can store its index
	 * @param <K> - key value
	 * @param <V> - value to store
	 */
	protected static class APQEntry<K, V> implements Entry<K, V> {

		private K k;
		private  V v;
		private int i;

		// Assign k, v, and i
		public APQEntry( K key, V value, int index ) {
			k = key;
			v = value;
			i = index;
		}

		public K getKey() {
			return k;
		}

		public V getValue() {
			return v;
		}

		public int getIndex() {
			return i;
		}

		protected void setKey( K key ) {
			k = key;
		}

		protected void setValue( V value ) {
			v = value;
		}

		// Public so index can be updated during element swaps
		public void setIndex( int index ) {
			i = index;
		}
	}

	// Create a comparator and ArrayList for the HeapAPQ to use
	private final Comparator<K> comp;
	protected ArrayList<Entry<K, V>> heap = new ArrayList<>( 100 );

	/* If no comparator is provided, use the default comparator which simply calls compareTo() method */
	public HeapAPQ() {
		this( new DefaultComparator<>() );
	}
	
	/* Use specified comparator */
	public HeapAPQ( Comparator<K> c ) {
		comp = c;
	}

	/* 
	 * Return the array in arraylist that is used to store entries  
	 * This method is purely for testing purpose of auto-grader
	 */
	public Object[] data() {
		// Use the testing method from the ArrayList class
		return heap.returnArray();
	}

	// Return the index of a given element's parent when given its index
	protected int parent( int i ) {
		return ( i - 1 ) / 2;
	}

	// Return the index of a given element's left child when given its index
	protected int left( int i ) {
		return i * 2 + 1;
	}

	// Return the index of a given element's right child index when given its index
	protected int right( int i ) {
		return i * 2 + 2;
	}

	// Check if the element at the given index has a left child
	protected boolean hasLeft( int i ) {
		return left( i ) < size();
	}

	// Check if the element at the given index has a right child
	protected boolean hasRight( int i ) {
		return right( i ) < size();
	}

	// Return the current size of the heap
	@Override
	@TimeComplexity ( "O(1)" )
	public int size() {
		return heap.size();
	}

	// Check if the heap is currently empty
	@Override
	@TimeComplexity ( "O(1)" )
	public boolean isEmpty() {
		return size() == 0;
	}

	/**
	 * Create a new element and insert it into the heap in the correct location
	 * @param key - the key of the new entry
	 * @param value - the associated value of the new entry
	 * @return - the newly added element
	 * @throws IllegalArgumentException - thrown if the key or value input is invalid
	 */
	@Override
	@TimeComplexity( "O(n)" )
	@TimeComplexityAmortized( "O(lg n)" )
	public Entry<K, V> insert( K key, V value ) throws IllegalArgumentException {
		/* TCJ
		 * Because the element is always inserted at the end of the list,
		 * the function call add( size(), ____ ) is O(1)
		 * The function call size() is O(1)
		 * The function call upheap() is O(lg n)
		 * The expected upper bound of the insert() method is O(lg n)
		 *
		 * If the heap's size equals the ArrayList's capacity, the ArrayList's
		 * size must be increased. When this occurs, all n elements must be
		 * copied over to the new array. Because of this process,
		 * the function call sizeUp() is O(n)
		 * The worst case of the insert() method is O(n)
		 */

		Entry<K, V> newest = new APQEntry<>( key, value, size() );
		heap.addLast( newest );
		upheap( size() - 1 );
		return newest;
	}

	// Returns the root entry of the heap
	@Override
	@TimeComplexity( "O(1)" )
	public Entry<K, V> min() {
		/* TCJ
		 * The function call isEmpty() is O(1)
		 * The function call heap.get( 0 ) is O(1) since it always
		 * grabs the first element, regardless of the size of n
		 * The worst case of the min() method is O(1)
		 */

		if( isEmpty() )
			return null;
		return heap.get( 0 );
	}

	// Removes and returns the root entry of the heap, then restores heap order
	@Override
	@TimeComplexity( "O(lg n)" )
	public Entry<K, V> removeMin() {
		/* TCJ
		 * The function call remove() is O(lg n)
		 * The worst case of the removeMin() method is O(lg n)
		 */

		if( isEmpty() )
			return null;
		Entry<K, V> removed = min();
		remove( min() );
		return removed;
	}

	/**
	 * Removes a specified element from the heap
	 * @param entry - an entry of this priority queue
	 * @throws IllegalArgumentException - thrown if the entry input is invalid
	 */
	@Override
	@TimeComplexity( "O(lg n)" )
	public void remove( Entry<K, V> entry ) throws IllegalArgumentException {
		/* TCJ
		 * The function call getIndex() is O(1)
		 * The function call size() is O(1)
		 * The function call heap.remove() is O(n)
		 * The function call swap() is O(1)
		 * The function call restoreHeap() is O(lg n)
		 * The worst case of the remove() method is O(n)
		 */

		if( isEmpty() )
			return;

		// Cast the entry back into an APQEntry so its index can be found
		APQEntry<K, V> locate = (APQEntry<K, V>) entry;
		int i = locate.getIndex();

		// Remove the element and restore heap order if necessary
		if( i == size() - 1 )
			heap.removeLast();
		else {
			swap( i, size() - 1 );
			heap.removeLast();
			restoreHeap( i );
		}
	}

	/**
	 * Update the key value of a specified element in the heap
	 * @param entry - an entry of this priority queue
	 * @param key - the new key
	 * @throws IllegalArgumentException - thrown if the entry or key input is invalid
	 */
	@Override
	@TimeComplexity( "O(lg n)" )
	public void replaceKey( Entry<K, V> entry, K key ) throws IllegalArgumentException {
		/* TCJ
		 * The function call setKey() is O(1)
		 * The function call restoreHeap() is O(lg n)
		 * The function call getIndex() is O(1)
		 * The worst case of the replaceKey() method is O(lg n)
		 */

		// Find and update the entry's key value, then restore heap order
		APQEntry<K, V> locate = (APQEntry<K, V>) entry;
		locate.setKey( key );
		restoreHeap( locate.getIndex() );
	}

	/**
	 * Update the stored value of a specified element in the heap
	 * @param entry - an entry of this priority queue
	 * @param value - the new value
	 * @throws IllegalArgumentException - thrown if the entry or value input is invalid
	 */
	@Override
	@TimeComplexity( "O(1)" )
	public void replaceValue( Entry<K, V> entry, V value ) throws IllegalArgumentException {
		/* TCJ
		 * The function call setValue() is O(1)
		 * The worst case of the replaceValue() method is O(1)
		 */

		// Find and update the entry's stored value (no subsequent order restoration necessary)
		APQEntry<K, V> locate = (APQEntry<K, V>) entry;
		locate.setValue( value );
	}

	/**
	 * Swaps the locations of two elements within the heap
	 * @param i - the index of the first element
	 * @param j - the index of the second element
	 */
	@TimeComplexity ( "O(1)" )
	protected void swap( int i, int j ) {
		/* TCJ
		 * The function call heap.get() is O(1)
		 * The function call heap.set() is O(1)
		 * The function call setIndex() is O(1)
		 * The worst case of the swap() method is O(1)
		 */

		// Swap the elements at the provided positions
		Entry<K, V> temp = heap.get( i );
		heap.set( i, heap.get( j ) );
		heap.set( j, temp );

		// Update the index values of the swapped elements
		( (APQEntry<K, V>) heap.get( i )).setIndex( i );
		( (APQEntry<K, V>) heap.get( j )).setIndex( j );
	}

	/**
	 * Streamlines the compare method for elements by preemptively retrieving their keys
	 * @param a - the first entry to be compared
	 * @param b - the second entry to be compared
	 * @return - the result of the comparison
	 */
	@TimeComplexity ( "O(1)" )
	protected int compare( Entry<K, V> a, Entry<K, V> b ) {
		/* TCJ
		 * The function call compare() is O(1)
		 * The function call getKey() is O(1)
		 * The worst case of the compare() method is O(1)
		 */

		return comp.compare( a.getKey(), b.getKey() );
	}

	/**
	 * Restore heap order by moving up through the tree
	 * @param i - the index to begin sorting at
	 */
	@TimeComplexity ( "O(lg n)" )
	protected void upheap( int i ) {
		/* TCJ
		 * An element and its parent are swapped until heap order is restored.
		 * The maximum number of swaps that can be made is the height of the
		 * heap, which is log(n) if n is the current size of the heap.
		 * The function call compare() is O(1)
		 * The function call heap.get() is O(1)
		 * The function call swap() is O(1)
		 * The worst case of the upheap method is O(lg n)
		 */

		while( i > 0 ) {
			int p = parent( i );

			// Stop the upheap if heap order has been restored
			if( compare( heap.get( i ), heap.get( p ) ) >= 0 )
				break;

			// Swap the element and its parent, then update i
			swap( i, p );
			i = p;
		}
	}

	/**
	 * Restore heap order by moving down through the tree
	 * @param i - the index to begin sorting at
	 */
	@TimeComplexity ( "O(lg n)" )
	protected void downheap( int i ) {
		/* TCJ
		 * An element and its minimal child are swapped until heap order is
		 * restored. The maximum number of swaps that can be made is the height
		 * of the heap, which is log(n) if n is the current size of the heap.
		 * The function call left() is O(1)
		 * The function call hasRight() is O(1)
		 * The function call right() is O(1)
		 * The function call compare() is O(1)
		 * The function call heap.get() is O(1)
		 * The function call swap() is O(1)
		 * The worst case of the downheap() method is O(lg n)
		 */

		while( hasLeft( i ) ) {
			// Find the minimal child element of i
			int leftIndex = left( i );
			int least = leftIndex;

			if( hasRight( i ) ) {
				int rightIndex = right( i );

				if( compare( heap.get( leftIndex ), heap.get( rightIndex ) ) >= 0 )
					least = rightIndex;
			}

			// Stop the downheap if heap order has been restored
			if( compare( heap.get( least ), heap.get( i ) ) >= 0 )
				break;

			// Swap the element and its least element, then update i
			swap( i, least );
			i = least;
		}
	}

	/**
	 * Restore heap order in the appropriate direction
	 * Universal method for when sorting direction is unknown
	 * @param i - the index to begin sorting at
	 */
	@TimeComplexity ( "O(lg n)" )
	protected void restoreHeap( int i ) {
		/* TCJ
		 * The function call compare() is O(1)
		 * The function call heap.get() is O(1)
		 * The function call upheap() is O(lg n)
		 * The function call downheap() is O(lg n)
		 * The worst case of the restoreHeap() method is O(lg n)
		 */

		if( i > 0 && compare( heap.get( i ), heap.get( parent( i ) ) ) < 0 )
			upheap( i );
		else
			downheap( i );
	}
}