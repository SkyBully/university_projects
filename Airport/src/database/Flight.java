package database;

public class Flight {
    private String id;
    private String from;
    private String to;
    private String time;

    public Flight(String id, String from, String to, String time) {
        this.id = id;
        this.from = from;
        this.to = to;
        this.time = time;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getFrom() {
        return from;
    }

    public void setFrom(String from) {
        this.from = from;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

}
