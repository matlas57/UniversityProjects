/**
 * Name: Matan Atlas
 * ID: 260943621
 * Code file 2 for COMP 350 Assignment 2 question 1
 */
public class SciNotNum
{
    /**
     * A data type to represent numbers in the specified format
     * @param f: represents the decimal number in standard decimal exponential format
     * @param k: represents the integer exponent, it is initially set to zero and will be modified by the constructor
     */
    private float f;
    private int k = 0;
    private float inf = Float.POSITIVE_INFINITY;
    private float lowestNorm = Float.MIN_NORMAL;

    /**
     * Constructor to create a new SciNotNum Object
     * @param orig: the floating point number in decimal format to be converted to standard decimal exponential format
     */
    public SciNotNum(float orig)
    {
        int exponent = 0;
        //if orig is greater than 1 then keep dividing by 10 and incrementing the exponent until the number is in standard decimal exponential format
        //if orig is 1 then there is no change and exponent doesn't change
        if (orig >= 1.0f)
        {
            while (orig / 10.0f > 1.0f)
            {
                exponent++;
                orig /= 10.0f;
            }
        }
        //if orig is less than 1 then keep multiplying by 10 and decrementing the exponent until the number is in standard decimal exponential format
        else
        {
            while (orig * 10.0f < 10.0f)
            {
                exponent--;
                orig *= 10.0f;
            }
        }
        this.f = orig;
        this.k = exponent;
    }

    /**
     * Alternate constructor to create SciNotNum from a product of two SciNotNum objects
     * @param orig: the floating point number from the product which may need to be converted into standard decimal exponential format
     * @param exp: the exponent computed from the multiplication which may need to be modified if a conversion is necessary
     */
    public SciNotNum(float orig, int exp)
    {
        //if orig is greater than 1 then keep dividing by 10 and incrementing the exponent until the number is in standard decimal exponential format
        //if orig is 1 then there is no change and exponent doesn't change
        if (orig >= 1.0f)
        {
            while (orig / 10.0f > 1.0f)
            {
                exp++;
                orig /= 10.0f;
            }
        }
        //if orig is less than 1 then keep multiplying by 10 and decrementing the exponent until the number is in standard decimal exponential format
        else
        {
            while (orig * 10.0f < 10.0f)
            {
                exp--;
                orig *= 10.0f;
            }
        }
        this.f = orig;
        this.k = exp;
    }


    public String display()
    {
        float evaluated = this.f * (float)(Math.pow(10.0, this.k));
        if (evaluated >= this.inf || evaluated <= this.lowestNorm)
        {
            return this.f + " times 10 to the power " + this.k;
        }
        else
        {
            return this.f * ((float)(Math.pow(10.0, this.k))) + " times 10 to the power 0";
        }
    }

    /**
     * This method multiplies two SciNotNum numbers to do this multiply the floating point components and add the exponent components
     * then create a new SciNotNum to ensure the product is in standard decimal exponential format
     * @param a: the first SciNotNum
     * @param b: the second SciNotNum
     * @return the product of a and b as a SciNotNum in standard decimal exponential format
     */
    public static SciNotNum Multiply(SciNotNum a, SciNotNum b)
    {
        float f = a.getF() * b.getF();
        int e = a.getK() + b.getK();
        return new SciNotNum(f, e);
    }

    /**
     * getter method for the f component of a SciNotNum used in Multiply
     * @return a SciNotNum's f component
     */
    public float getF() {
        return f;
    }

    /**
     * getter method for the k component of a SciNotNum used in Multiply
     * @return a SciNotNum's k component
     */
    public int getK() {
        return k;
    }
}
