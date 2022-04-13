import java.lang.management.PlatformLoggingMXBean;
import java.util.*;

/**
 * Represents a sequence of playables to play in FIFO order.
 */
public class PlayList implements Playable, UndoRedo {

    private List<Playable> aList = new LinkedList<>();
    private String aName;

    // Fields for undo and redo functionality
    private Stack<PlayList> prevPlaylists = new Stack<>();
    private boolean redoMethod;                             // if this value is true redo() will execute the last method called again, if false it will redo the last undo
    private RedoEncoding redoCode = new RedoEncoding();
    private Stack<PlayList> undoneActions = new Stack<>();

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
    }

    /**
     * Adds a playable at the end of this playlist.
     *
     * @param pPlayable
     *            the content to add to the list
     * @pre pPlayable!=null;
     */
    public void addPlayable(Playable pPlayable) {
        assert pPlayable != null;
        this.StoreState(prevPlaylists);
        redoMethod = true;
        RedoEncoding re = new RedoEncoding(StateChangers.ADD, Optional.empty(), Optional.of(pPlayable));
        this.redoCode = re;
        aList.add(pPlayable);
    }

    /**
     * remove a playable from the Playlist given its index
     * @param pIndex
     *          the index of playable to be removed
     * @return the removed playable
     */
    public Playable removePlayable(int pIndex) {
        assert pIndex >= 0 && pIndex < aList.size();
        this.StoreState(prevPlaylists);
        redoMethod = true;
        RedoEncoding re = new RedoEncoding(StateChangers.REMOVE, Optional.of(pIndex), Optional.empty());
        this.redoCode = re;
        return aList.remove(pIndex);
    }

    /**
     * @return The name of the playlist.
     */
    public String getName() {
        return aName;
    }

    /**
     * modify the name of the playlist
     * @param pName
     *          the new name of the playlist
     */
    public void setName(String pName) {
        assert pName != null;
        this.StoreState(prevPlaylists);
        redoMethod = true;
        RedoEncoding re = new RedoEncoding(StateChangers.SETNAME, Optional.empty(), Optional.empty());
        this.redoCode = re;
        this.aName = pName;
    }

    /**
     * Iterating through the playlist to play playable content.
     */
    @Override
    public void play() {
        for(Playable playable:aList){
            playable.play();
        }
    }

    /**
     * change the order how playlist play the playables it contains
     */
    public void shuffle() {
        this.StoreState(prevPlaylists);
        redoMethod = true;
        RedoEncoding re = new RedoEncoding(StateChangers.SHUFFLE, Optional.empty(), Optional.empty());
        this.redoCode = re;
        Collections.shuffle(aList);
    }

    /**
     * Checks is two playlists are equal based on playable objects and their order
     *
     * @param o
     *            The object to compare a playlist to
     * @return    This method returns true if the playlist is the same as the obj argument; false otherwise.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PlayList playList = (PlayList) o;
        return this.aList.equals(playList.aList);
    }

    /**
     * Equal playlists have the same hashcode
     */
    @Override
    public int hashCode() {
        return Objects.hash(aList);
    }

    public int getSize()
    {
        return this.aList.size();
    }

    public List<Playable> getaListCopy()
    {
        return Collections.unmodifiableList(aList);
    }

    private void StoreState(Stack location)
    {
        // storing the current state of the Playlist in the shortcuts stack so the action can be undone
        PlayList curState = new PlayList(this.aName);
        List<Playable> curList = new LinkedList<>();
        for (Playable p : this.aList)
        {
            curList.add(p);
        }
        curState.aList = curList;
        location.push(curState);
    }


    @Override
    public void undo()
    {
        this.StoreState(undoneActions);
        PlayList old = prevPlaylists.pop();
        aList = old.aList;
        aName = old.aName;
        redoMethod = false;
    }

    @Override
    public void redo()
    {
        if (redoMethod)
        {
            System.out.println("Executing the last method");
            this.StoreState(prevPlaylists);
            if (redoCode.getLastMethodCalled() == StateChangers.ADD)
            {
                Optional<Playable> p = redoCode.getOptionalPlayable();
                if (!p.isEmpty())
                {
                    Playable add = p.get();
                    this.addPlayable(add);
                }

            }
            else if (redoCode.getLastMethodCalled() == StateChangers.REMOVE)
            {
                if (redoCode.getOptionalIndex().isPresent() && redoCode.getOptionalIndex().get() < this.aList.size())
                {
                    this.removePlayable(redoCode.getOptionalIndex().get());
                }
            }
            else if (redoCode.getLastMethodCalled() == StateChangers.SHUFFLE)
            {
                this.shuffle();
            }
            // if the last state changing method was setName there is nothing to be done if redo is called because redo would just setName again and the call would be redundant
        }
        else
        {
            this.StoreState(prevPlaylists);
            PlayList revert = undoneActions.pop();
            aList = revert.aList;
            aName = revert.aName;
        }
    }
}
