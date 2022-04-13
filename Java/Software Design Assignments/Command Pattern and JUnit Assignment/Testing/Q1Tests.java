import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class Q1Tests {

    @Test
    void TestSetDefault()
    {
        Library.createLibrary("Library");
        Library lib = Library.getLibrary();
        Song s1 = new Song("S1", "A1");
        lib.setDefaultPlayable(s1);

        assertEquals(s1, lib.getDefaultClone());
    }

}
