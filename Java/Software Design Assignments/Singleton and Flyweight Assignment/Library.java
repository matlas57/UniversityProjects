import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Optional;

/**
 * Represents an Audio library, with individual Song titles, Podcasts and playlists.
 */
public class Library {

    /**
     * Attributes of the class library
     */
    private String name; // the name of the library, figure out how to allow the client to name this while maintaining singleton
    private Optional<String> lang = Optional.empty(); // make optional
    private ArrayList<Song> songs = new ArrayList<>(); // song storage in the library
    private ArrayList<PlayList> playLists  = new ArrayList<>(); // playlist storage in the library
    private ArrayList<Podcast> podcasts  = new ArrayList<>(); // podcasts storage in the library
    private ArrayList<Episode> episodes = new ArrayList<>(); // podcast episode storage in the library
    /**
     * Single instance of the library object using the singleton design pattern
     * it is initially set to null so that the getLibrary_instance method can check if it needs to create the instance when called
     */
    private static Library library_instance = null;

    /**
     * a private constructor, necessary for implementing singleton library
     * @param pName : the name for the library
     * @param pLang : the language of the library
     */
    private Library(String pName, Optional<String> pLang)
    {
        this.name = pName;
        this.lang = pLang;
    }

    /**
     * method to provide/create library instance. if there is no library yet created one will be instantiated, if there is already one created it will be returned
     * @return : returns either a newly created library or already existing library
     */
    public static Library getLibrary_instance(String pName, Optional<String> pLang)
    {
        if (library_instance == null)
        {
            library_instance = new Library(pName, pLang);
            return library_instance;
        }
        System.out.println("There is already an instance of a library");
        return library_instance;
    }

    /**
     * Adds a Song to the library. Duplicate Songs aren't added twice.
     *
     * @param pSong the Song to add
     */
    public void addSong(Song pSong) {
        assert pSong != null;
        for (Song s : songs)
        {
            if (s.equals(pSong)) {
                System.out.println("The song is already contained in the library " + this.name + ".");
                return;
            }
        }
        songs.add(pSong);
    }

    /**
     * Adds a PlayList to the library. All Songs from the list are also added as individual Songs to the library.
     *
     * @param pList
     *            the PlayList to add
     * @pre pList!=null;
     */
    public void addPlayList(PlayList pList) {
       assert pList != null;
       playLists.add(pList);
       int index = 0;
       while (true)
       {
           try
           {
               Playable p = pList.getPlayable(index);
               if (p instanceof Song)
               {
                   songs.add((Song) p);
               }
               else
               {
                   episodes.add((Episode) p);
               }
               index++;
           }
           catch (AssertionError e)
           {
               System.out.println("All items from playlist added to library");
               break;
           }
       }
    }

    /**
     * Adds a Podcast to the library. All Episodes from the list are also added as individual episodes to the library.
     *
     * @param pPodcast
     *            the Podcast to add
     * @pre pPodcast!=null;
     */
    public void addPodcast(Podcast pPodcast)
    {
        assert pPodcast != null;
        for (Podcast p : podcasts)
        {
            if (p.equals(pPodcast))
            {
                System.out.println("This podcast is already contained in the library");
                return;
            }
        }
        podcasts.add(pPodcast);
        int index = 0;
        /*
         * because the only way to access episodes of the podcast is through the getEpisode() method we don't have access to the amount of episodes
         * use a while loop to try each episode and catch and out of bound exception which will signal when all episodes have been added
         */
        while (true)
        {
            try
            {
                episodes.add(pPodcast.getEpisode(index));
                //System.out.println("added episode " + pPodcast.getEpisode(index).getaTitle() + " to the library");
                index++;
            }
            catch (IndexOutOfBoundsException e)
            {
                System.out.println("All episodes from the podcast were added to the library");
                break;
            }
        }
    }

    /**
     * for testing purposes to show that when a podcast is added to the library so are all the individual episodes
     * @return size of the episodes list in the library
     */
    public int getNumEpisodes()
    {
        return this.episodes.size();
    }
    /**
     * for testing purposes to show that when a playlist is added to the library so are all the individual songs
     * @return size of the songs list in the library
     */
    public int getNumSongs() {return this.songs.size();}

}
