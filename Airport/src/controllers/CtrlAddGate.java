package controllers;

import database.Database;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class CtrlAddGate {
    private static String columns = "(name_gate)";
    private static String table= "public.gate";


    @FXML
    private TextField nameOfGate;

    public void addNewGate(ActionEvent actionEvent) {
        Database database = new Database();
        database.addData(table,columns, "'" + nameOfGate.getText() + "'");
        Stage stage = (Stage) nameOfGate.getScene().getWindow();
        stage.close();
    }
}
