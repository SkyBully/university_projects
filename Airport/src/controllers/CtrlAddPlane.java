package controllers;

import database.Database;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class CtrlAddPlane {
    private static String columns = "(name_plane,aircraft_registration)";
    private static String table= "public.plane";

    @FXML
    private TextField nameOfPlane;
    @FXML
    private TextField airReg;

    public void AddNewPlane(ActionEvent actionEvent) {
        Database database = new Database();
        database.addData(table,columns, "'" + nameOfPlane.getText() + "', '" + airReg.getText() + "'");
        Stage stage = (Stage) nameOfPlane.getScene().getWindow();
        stage.close();
    }
}