import org.junit.jupiter.api.Test;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class Q2Tests {

    @Test
    void TestUndoAddPlayable() {
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        Song s2 = new Song("S2", "A2");

        p.addPlayable(s1);
        p.addPlayable(s2);
        p.undo();

        assertEquals(1, p.getSize());
    }

    @Test
    void TestUndoRemovePlayable() {
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        Song s2 = new Song("S2", "A2");

        p.addPlayable(s1);
        p.addPlayable(s2);
        p.removePlayable(0);
        p.undo();

        assertEquals(2, p.getSize());
    }

    @Test
    void TestUndoSetName() {
        PlayList p = new PlayList("Playlist");
        p.setName("New Name");

        p.undo();

        assertEquals("Playlist", p.getName());
    }

    @Test
    void TestUndoShuffle() {
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        Song s2 = new Song("S2", "A2");
        Song s3 = new Song("S3", "A3");
        p.addPlayable(s1);
        p.addPlayable(s2);
        p.addPlayable(s3);

        List<Playable> oracle = new LinkedList();
        oracle.add(s1);
        oracle.add(s2);
        oracle.add(s3);

        p.shuffle();
        p.undo();

        assertEquals(oracle, p.getaListCopy());
    }

    @Test
    void TestUndoRedo(){
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        p.addPlayable(s1);
        p.redo();
        p.undo();

        assertEquals(1, p.getaListCopy().size());
    }

    @Test
    void TestRedoUndo(){
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        Song s2 = new Song("S2", "A2");
        p.addPlayable(s1);
        p.addPlayable(s2);
        p.undo();
        p.redo();

        assertEquals(2, p.getaListCopy().size());
    }

    @Test
    void TestRedoAdd(){
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        p.addPlayable(s1);
        p.redo();

        assertEquals(2, p.getaListCopy().size());
    }

    @Test
    void TestRedoRemove(){
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        Song s2 = new Song("S2", "A2");
        Song s3 = new Song("S3", "A3");
        p.addPlayable(s1);
        p.addPlayable(s2);
        p.addPlayable(s3);
        p.removePlayable(0);
        p.redo();

        assertEquals(1, p.getaListCopy().size());
    }

    @Test
    void TestRedoShuffle(){
        PlayList p = new PlayList("Playlist");
        Song s1 = new Song("S1", "A1");
        Song s2 = new Song("S2", "A2");
        Song s3 = new Song("S3", "A3");
        p.addPlayable(s1);
        p.addPlayable(s2);
        p.addPlayable(s3);
        p.shuffle();
        List<Playable> oracle = new LinkedList();
        for (Playable playables : p.getaListCopy())
        {
            oracle.add(playables);
        }
        p.redo();

        assertNotEquals(oracle, p.getaListCopy());
    }

}