package database;

public class Baggage {
    private String id;
    private String weight;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getWeight() {
        return weight;
    }

    public void setWeight(String weight) {
        this.weight = weight;
    }

    public Baggage(String id, String weight) {
        this.id = id;
        this.weight = weight;
    }
}
