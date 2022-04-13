import java.util.Optional;

public class Main {
    public static void main(String[] args) {
        System.out.println("Test 1: Singleton Library with optional language parameter");
        // demonstrating that library requires a name and may take a language but optional type is used so that client may or may not use a language
        Library lib = Library.getLibrary_instance("Library", Optional.of("English"));
        // demonstrating singleton library: next line will try to create a new library, program will print "There is already an instance of a library"
        Library lib2 = Library.getLibrary_instance("Library", Optional.empty());
        System.out.println();
        System.out.println("Test 2: Flyweight songs with case insensitivity when creating song instances");
        /*
         * demonstrating Flyweight for songs: songDuplicate will try to create a duplicate song but the creation will be prevented and program will print
         * "Found an instance of the song [Title]"
         * also demonstrating that equality for songs is case-insensitive: song has capitalization while songDuplicate does not,
         * the program still identifies songDuplicate to be a duplicate
         */
        Song song = Song.getSongInstance("Coding Song", "Matan Atlas");
        Song songDuplicate = Song.getSongInstance("coding song", "matan atlas");
        System.out.println();
        System.out.println("Test 3: Flyweight podcasts with case insensitivity when creating podcast instances");
        /*
         * demonstrating Flyweight for podcast: podcastDuplicate will try to create a duplicate podcast but the creation will be prevented and program will print
         * "Found an instance of the podcast [Title]"
         * also demonstrating that equality for podcasts is case-insensitive: podcast has capitalization while podcastDuplicate does not,
         */
        Podcast podcast = Podcast.getPodcastInstance("Coding Podcast", "Matan Atlas");
        Podcast podcastDuplicate = Podcast.getPodcastInstance("coding podcast", "matan atlas");
        System.out.println();
        System.out.println("Test 4: Playlists cant contain null objects or other playlists");
        /*
         * demonstrating that null playable objects can't be added to a playlist when assertions are enabled
         * demonstrating that playlists can't be added to a playlist when assertions are enabled
         */
        PlayList playList1 = new PlayList("playlist1");
        Song nullSong = null;
        try
        {
            playList1.addPlayable(nullSong);
        }
        catch (AssertionError e)
        {
            System.out.println("Cant add null object to a playlist");
        }
        PlayList playList2 = new PlayList("playlist2");
        try
        {
            playList1.addPlayable(playList2);
        }
        catch (AssertionError e)
        {
            System.out.println("Cant add a playlist to another playlist");
        }
        System.out.println();
        System.out.println("Test 5: two playlists are equal if they contain the same Playable objects in the same order regardless of name");
        /*
         * Demonstrating that two playlists will be seen as equal if they have the same objects in the same order, even if their names are different
         * equalPlaylist1 and equalPlaylist2 contain the same two objects in the same order and have a different name, they will be seen as equal
         * nonEqualPlaylist has the same two objects as the equal playlists but in different order, it will not be equal to the other two playlists
         */
        PlayList equalPlaylist1 = new PlayList("equal1");
        PlayList equalPlaylist2 = new PlayList("equal2");
        PlayList nonEqualPlaylist = new PlayList("nonequal");
        Episode episode1 = new Episode(podcast, "Software Design", 1);
        equalPlaylist1.addPlayable(song);
        equalPlaylist2.addPlayable(song);
        equalPlaylist1.addPlayable(episode1);
        equalPlaylist2.addPlayable(episode1);
        nonEqualPlaylist.addPlayable(episode1);
        nonEqualPlaylist.addPlayable(song);
        System.out.println(equalPlaylist1.equals(equalPlaylist2)); // prints true since the podcasts are equal
        System.out.println(equalPlaylist1.equals(nonEqualPlaylist)); // prints false since the podcasts contain their elements in different order
        System.out.println();
        System.out.println("Test 6: adding a podcast to a library also adds all episodes to the library");
        /*
         * Demonstrate that when the podcast is added to the library the size of the episode list grows meaning that the episodes of the podcast have been added to the library
         */
        Podcast podcast1 = Podcast.getPodcastInstance("Software Design Podcast", "Matan Atlas");
        Episode ep1 = new Episode(podcast1, "ep1", 1);
        Episode ep2 = new Episode(podcast1, "ep2", 2);
        podcast1.addEpisode(ep1);
        podcast1.addEpisode(ep2);
        System.out.println(lib.getNumEpisodes()); // size of the episode list before the podcast is added to the library
        lib.addPodcast(podcast1);
        System.out.println(lib.getNumEpisodes()); // size of the episode list after the podcast is added to the library
        System.out.println();
        System.out.println("Test 6: adding a playlist to a library also adds all playables in the playlist to the library");
        /*
         * Demonstrate that when a playlist is added to the library the size of the episode list and the size of the song list grows
         */
        System.out.println(lib.getNumEpisodes()); // print the number of episodes before a playlist is added: prints 2
        System.out.println(lib.getNumSongs()); // print the number of songs before a playlist is added: prints 0
        PlayList playlist = new PlayList("playlist");
        Song s1 = Song.getSongInstance("Coding Is Fun", "Coder Person");
        Song s2 = Song.getSongInstance("Coding is not fun", "Not Coder Person");
        Podcast podcast2 = Podcast.getPodcastInstance("Coding is Fun Podcast", "Coder Person");
        Episode e1 = new Episode(podcast2, "Coding is so much fun", 1);
        playlist.addPlayable(s1);
        playlist.addPlayable(s2);
        playlist.addPlayable(e1);
        lib.addPlayList(playlist);
        System.out.println(lib.getNumEpisodes()); // 1 episode is added: prints 3 = 2 + 1
        System.out.println(lib.getNumSongs()); // 2 songs are added: prints 2 = 0 + 2
    }
}
