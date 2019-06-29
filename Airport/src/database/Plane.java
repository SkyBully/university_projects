package database;

public class Plane {
    private String id;
    private String name;
    private String aircraftRegistration;

    public Plane(String id, String name, String aircraftRegistration) {
        this.id = id;
        this.name = name;
        this.aircraftRegistration = aircraftRegistration;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAircraftRegistration() {
        return aircraftRegistration;
    }

    public void setAircraftRegistration(String aircraftRegistration) {
        this.aircraftRegistration = aircraftRegistration;
    }
}
