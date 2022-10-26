/**
 * Name: Matan Atlas
 * ID: 260943621
 * Code file 1 for COMP 350 Assignment 2 question 1
 */
public class Q1 {
    public static String ComputeProduct(float[] input)
    {
        SciNotNum product = new SciNotNum(1.0f);
        for (float f : input)
        {
            if (f != input[input.length - 1])
            {
                System.out.print(f + " * ");
            }
            else
            {
                System.out.print(f + " = ");
            }
            SciNotNum cur = new SciNotNum(f);
            product = SciNotNum.Multiply(product, cur);
        }
        System.out.println(product.display());
        return product.display();
    }
}
