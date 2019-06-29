package controllers;

import database.Database;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class CtrlAddFlight {
    private static String columns = "(\"from\",\"to\",\"time\")";
    private static String table= "public.flight";
    @FXML
    private TextField flightFrom;
    @FXML
    private TextField flightTo;
    @FXML
    private TextField flightTime;

    public void AddNewFlight(ActionEvent actionEvent) {
        Database database = new Database();
        database.addData(table,columns, "'" + flightFrom.getText() + "', '" + flightTo.getText() + "', '" + flightTime.getText() + "'");
        Stage stage = (Stage) flightFrom.getScene().getWindow();
        stage.close();
    }
}
