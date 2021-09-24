package cs2321;

public class HeapPQSort<E extends Comparable<E>> implements Sorter<E> {

	/**
	 * Sort - perform a PriorityQueue sort using the heap implementation of PQ.
	 * @param array - Array to sort
	 */
	@TimeComplexity( "O(n lg n)" )
	public void sort( E[] array ) {
		/* TCJ
		 * Every element in the input array is inserted into the HeapPQ, then
		 * the min is removed until it is empty. These actions themselves are
		 * O(1), but the resulting upheap and downheap functions are O(lg n).
		 * Furthermore, if the PQ's size limit has been reached, sizeUp() must
		 * be called, which has a complexity of O(n).
		 * The function call insert() is O(n)
		 * The function call removeMin() is O(lg n)
		 * The function call getKey() is O(1)
		 * The worst case of the sort() method is O(n lg n)
		 */

		HeapAPQ<E, E> heap = new HeapAPQ();

		// Insert every element into the HeapPQ
		for( E e : array )
			heap.insert( e, null );

		// Remove min from the PQ until it is empty
		for( int i = 0; i < array.length; i++ )
			array[ i ] = heap.removeMin().getKey();
	}
}