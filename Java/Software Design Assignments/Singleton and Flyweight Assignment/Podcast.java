import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Represents a single Podcast, with at least a name and a host. Each Podcast aggregates episodes.
 */
public class Podcast{

    private String aName;
    private String aHost;
    private List<Episode> aEpisodes = new ArrayList<>();
    private static ArrayList<Podcast> podcastCache = new ArrayList<>(); // list of podcasts that belongs to podcast class that stores all already created podcasts so no duplicates are created

    /**
     * Creates a Podcast
     * Private constructor to prevent duplicate podcast creation
     *
     */
    private Podcast(String pName, String pHost)
    {
        this.aName = pName;
        this.aHost = pHost;
    }

    public static Podcast getPodcastInstance(String pName, String pHost)
    {
        for (Podcast p : podcastCache)
        {
            if (p.aName.toLowerCase().equals(pName.toLowerCase()) && p.aHost.toLowerCase().equals(pHost.toLowerCase()))
            {
                System.out.println("Found an instance of the podcast " + pName);
                return p;
            }
        }
        System.out.println("Creating a new podcast object with name "+ pName);
        Podcast p = new Podcast(pName, pHost);
        podcastCache.add(p);
        return p;
    }


    /**
     * Add one episode to the podcast
     * @param pEpisode to be put into the podcast
     * @pre
     */
    protected void addEpisode(Episode pEpisode) {
        if(!aEpisodes.contains(pEpisode)) {
            aEpisodes.add(pEpisode);
        }
    }

    /**
     * retrieve one episode from the podcast
     * @param pIndex
     *
     */
    public Episode getEpisode(int pIndex) {
        return this.aEpisodes.get(pIndex);
    }

    public String getName() {
        return aName;
    }

    public String getaHost() {
        return aHost;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Podcast podcast = (Podcast) o;
        return Objects.equals(aName.toLowerCase(), podcast.aName.toLowerCase()) && Objects.equals(aHost.toLowerCase(), podcast.aHost.toLowerCase());
    }

    @Override
    public int hashCode() {
        return Objects.hash(aName, aHost);
    }
}