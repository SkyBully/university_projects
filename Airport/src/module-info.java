module DBS {
    requires javafx.fxml;
    requires javafx.controls;
    requires java.sql;
    opens sample;
    opens view;
    opens controllers;
    opens database;
}