/**
 * Name: Matan Atlas
 * ID: 260943621
 * Class for question 2 of assignment 2 for COMP 350.
 * Task: compute pn - pi for n =[3...20] recursively.
 */
public class Q2 {
    private static final float base = 2.0f * (float) Math.sqrt(2.0);
    /**
     * recursive method to compute p(n).
     * The issue with this method of calculating p(n) - pi is that when n = 15, 16, ... , 20
     * p(n) is rounded down to zero and the value returned is simply -pi
     * I cast all operations that use double data type to float for sake of consistency and
     * to ensure that the primitive operations are done with single precision floats
     *
     * @param n the current value of p(n) to be computed.
     * @return the computed value p(n).
     */
    public static float computePerimeter(int n) {
        if (n == 2) {
            return base;
        }
        return (float) Math.pow(2.0, n - 1)
                * (float) Math.sqrt(2.0f
                * (1.0f - (float) Math.sqrt(1.0f
                - (float) Math.pow((computePerimeter(n - 1)
                / (float) Math.pow(2.0, n - 1)), 2.0))));
    }

    /**
     * recursive method to compute pn using the revised formula created through rationalization.
     * rationalization eliminates the subtraction operations that caused a rounding error to zero for p(n) at large n in the original formula
     *
     * @param n the current value of p(n) to be computed.
     * @return the computed value p(n).
     */
    public static float computePerimeterAdjusted(int n) {
        if (n == 2){
            return base;
        }
        return (float) ((( (float) Math.pow(2.0, ((2.0 * n) - 2))) // Numerator part 1: 2^(2n-2)
                *  (float) Math.sqrt((4.0f * (float) Math.pow((computePerimeterAdjusted(n - 1) / (float) Math.pow(2.0, n - 1)), 2.0)))) // Numerator part 2: SQRT(4 * (p(n-1)/n^(n-1))^2)
                / (Math.pow(2.0, n - 1) // Denominator part 1: 2^(n-1)
                * Math.sqrt(2.0f + (2.0f * Math.sqrt(1.0 - Math.pow((computePerimeterAdjusted(n - 1) / Math.pow(2.0, n - 1)), 2.0)))))); // Denominator part 2: SQRT(2 * (1 + SQRT(1 - (p(n-1)/2^(n-1))^2)))
    }

    /**
     * main method.
     *
     * @param args system arguments
     */
    public static void main(String[] args) {
        System.out.println("Below is the output of the original formula used to compute p(n)");
        for (int n = 3; n < 21; n++) {
            System.out.println(" n = " + n + " p(n) - pi = " + ((computePerimeter(n)) - (float) Math.PI));
        }
        System.out.println("Below is the output of the revised formula used to compute p(n)");
        for (int n = 3; n < 21; n++) {
            System.out.println(" n = " + n + " p(n) - pi = " + ((computePerimeterAdjusted(n)) - (float) Math.PI));
        }
    }
}
