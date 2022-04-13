import java.util.Optional;

public class RedoEncoding
{
    private StateChangers lastMethodCalled;
    private Optional<Integer> optionalIndex;
    private Optional<Playable> optionalPlayable;

    public RedoEncoding()
    {
        this.lastMethodCalled = null;
        this.optionalIndex = Optional.empty();
        this.optionalPlayable = Optional.empty();
    }

    public RedoEncoding(StateChangers sc, Optional<Integer> idx, Optional<Playable> playable)
    {
        this.lastMethodCalled = sc;
        this.optionalIndex = idx;
        this.optionalPlayable = playable;

    }

    public StateChangers getLastMethodCalled()
    {
        return lastMethodCalled;
    }

    public Optional<Integer> getOptionalIndex() {
        return optionalIndex;
    }

    public Optional<Playable> getOptionalPlayable() {
        return optionalPlayable;
    }
}
