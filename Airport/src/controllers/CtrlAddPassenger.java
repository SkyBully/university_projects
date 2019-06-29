package controllers;

import database.Database;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.DatePicker;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class CtrlAddPassenger {

    private static String columns = "(first_name, last_name, email, passport_id, date_of_birth, phone_number)";
    private static String table= "public.passenger";

    @FXML
    private TextField firstName;
    @FXML
    private TextField lastName;
    @FXML
    private TextField email;
    @FXML
    private TextField passportID;
    @FXML
    private TextField phoneNumber;
    @FXML
    private DatePicker dateOfBirth;
    public void addNewPassenger(ActionEvent actionEvent) {
        Database database = new Database();
        database.addData(table,columns, "'" + firstName.getText() + "', '" + lastName.getText() + "', '" + email.getText() + "', '" + passportID.getText() + "', '" + dateOfBirth.getValue() + "', '"+ phoneNumber.getText() + "'");
        //System.out.println(dateOfBirth.getValue());
        Stage stage = (Stage) firstName.getScene().getWindow();
        stage.close();
    }
}
