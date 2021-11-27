package cs2321;

import net.datastructures.Entry;

/**
 * @author - Ryan Cole
 * This class solves the fractional knapsack problem using a Priority Queue
 */
public class FractionalKnapsack {

	/**
	 * Goal: Choose items with maximum total benefit but with weight at most W.
	 *       You are allowed to take fractional amounts from items.
	 *
	 * @param items items[i][0] is weight for item i
	 *              items[i][1] is benefit for item i
	 * @param knapsackWeight - the maximum weight the knapsack can carry
	 * @return The maximum total benefit. Please use double type operation. For example 5/2 = 2.5
	 *
	 */
	public static double MaximumValue( int[][] items, int knapsackWeight ) {
		// Create a PQ and necessary counting variables
		HeapAPQ<Double, Double> knapsack = new HeapAPQ<>();
		double totalWeight = 0.0;
		double totalBenefits = 0.0;

		// Insert each possible item into the PQ
		for ( int[] item : items ) {
			double value = (double) item[1] / item[0];
			knapsack.insert( 1 / value, (double) item[0] );
		}

		// Place items in the bag until it is full or no items remain
		while( totalWeight < knapsackWeight && !knapsack.isEmpty() ) {
			// Place the maximal amount of the item with the next best value per unit in the bag
			Entry<Double, Double> carryItem = knapsack.removeMin();
			double amountToCarry = Math.min( carryItem.getValue(), knapsackWeight - totalWeight );

			// Update the total benefit and weight of the items in the bag
			totalBenefits += ( 1 / carryItem.getKey() ) * amountToCarry;
			totalWeight += amountToCarry;
		}

		return totalBenefits;
	}
}