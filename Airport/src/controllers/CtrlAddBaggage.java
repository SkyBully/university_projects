package controllers;

import database.BoardingPass;
import database.Database;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;

public class CtrlAddBaggage {
    @FXML
    public TextField weight;
    @FXML
    public Label boardingPass;

    private  CtrlEditBaggage parent;

    public void setParent(CtrlEditBaggage parent, String id) {
        boardingPass.setText(id);
        this.parent = parent;
    }

    public void apply(ActionEvent actionEvent) throws SQLException {
        Database database = new Database();
        database.addDataTransaction("public.baggage","(id_boarding_pass,weight)", "'" + boardingPass.getText() + "', '" + weight.getText() + "'", "; UPDATE public.boardingpass SET price = price + 30.00 WHERE id_boarding_pass =" + boardingPass.getText());
        ResultSet result = database.getData("public.boardingpass bp JOIN public.passenger p ON bp.id_passenger=p.id_passenger","id_boarding_pass = " + boardingPass.getText());
        while(result.next()){
            parent.setPriceTextField(result.getString("price") + ",00 EUR");
        }

        Stage stage = (Stage) boardingPass.getScene().getWindow();
        stage.close();
    }
}
