import java.util.LinkedList;
import java.util.List;
import java.util.Objects;

/**
 * Represents a sequence of playables to play in FIFO order.
 */
public class PlayList implements Playable {

    private final List<Playable> aList = new LinkedList<>();
    private String aName;
    private int aNext;

    /**
     * Creates a new empty playlist.
     *
     * @param pName
     *            the name of the list
     * @pre pName!=null;
     */
    public PlayList(String pName) {
        assert pName != null;
        aName = pName;
        aNext = 0;
    }

    /**
     * Adds a playable at the end of this playlist.
     *
     * @param pPlayable
     *            the content to add to the list
     * @pre pPlayable!=null;
     */
    public void addPlayable(Playable pPlayable) {
        assert pPlayable != null && !(pPlayable instanceof PlayList);
        aList.add(pPlayable);
    }

    public Playable getPlayable(int index)
    {
        assert index >= 0 && index < aList.size();
        return aList.get(index);
    }


    @Override
    public void play() {
        for (Playable p : aList)
        {
            p.play();
        }
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PlayList playList = (PlayList) o;
        return Objects.equals(aList, playList.aList);
    }

    @Override
    public int hashCode() {
        return Objects.hash(aList);
    }
}
