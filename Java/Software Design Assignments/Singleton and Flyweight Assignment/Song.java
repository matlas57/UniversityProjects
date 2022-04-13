import java.util.ArrayList;
import java.util.Objects;

/**
 * Represents a single Song, with at least a title, and an artist name.
 */
public class Song implements Playable  {

    private String aTitle;
    private String aArtist;
    private static ArrayList<Song> songCache = new ArrayList<>();
    /**
     * Creates a Song.
     * the constructor is private so that duplicate song objects can't be created,
     * instead song object requests are handles by getSongInstance()
     * Here I am using FLYWEIGHT for Song
     */
    private Song (String pTitle, String pArtist)
    {
        this.aTitle = pTitle;
        this.aArtist = pArtist;
    }


    /**
     * @param pTitle : the title of the song that will be either created or retrieved from the song cache
     * @param pArtist: the artist of the song that will be either created or retrieved from the song cache
     * @return returns a song with the passes title and artist, if the song has already been created that object will be returned.
     * if there does not exist an instance of a song with the title and artist a new song object is created using the constructor
     */
    public static Song getSongInstance(String pTitle, String pArtist)
    {
        for (Song s : Song.songCache)
        {
            if (s.aTitle.toLowerCase().equals(pTitle.toLowerCase()) && s.aArtist.toLowerCase().equals(pArtist.toLowerCase()))
            {
                System.out.println("Found an instance of the song " + pTitle);
                return s;
            }
        }
        System.out.println("Creating new song object with title " + pTitle);
        Song s = new Song(pTitle, pArtist);
        Song.songCache.add(s);
        return s;
    }


    public void play() {
        // Just a stub.
        // We don't expect you to implement an actual music player!
        System.out.println("Now playing " + aTitle);
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Song song = (Song) o;
        return Objects.equals(aTitle.toLowerCase(), song.aTitle.toLowerCase()) && Objects.equals(aArtist.toLowerCase(), song.aArtist.toLowerCase());
    }

    @Override
    public int hashCode() {
        return Objects.hash(aTitle, aArtist);
    }
}