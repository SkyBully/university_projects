package controllers;

import database.Database;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.DatePicker;
import javafx.scene.control.TextField;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import java.io.IOException;

public class CtrlAddFly {
    private static String columns = "(id_flight,id_gate,id_plane,date_fly)";
    private static String table= "public.fly";
    @FXML
    public TextField idFlight;
    @FXML
    public TextField idGate;
    @FXML
    public TextField idPlane;
    @FXML
    public DatePicker dateOfFly;

    private CtrlSearchPlane childrenPlane;
    private CtrlSearchGate childrenGate;
    private CtrlSearchFlight childrenFlight;

    public void setPlaneTextField(String id){
        idPlane.setText(id);
    }

    public void setGateTextField(String id){
        idGate.setText(id);
    }

    public void setFlightTextField(String id){
        idFlight.setText(id);
    }

    public void searchFlight(ActionEvent actionEvent) {
        try {
            Stage stage = new Stage();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/view/search_flight_form.fxml"));
            Parent root = (Parent) loader.load();
            stage.setScene(new Scene(root));
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
            stage.show();
            childrenFlight = loader.getController();
            childrenFlight.setParent(this);
        } catch (IOException e){
            e.printStackTrace();
        }
    }

    public void searchGate(ActionEvent actionEvent) {
        try {
            Stage stage = new Stage();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/view/search_gate_form.fxml"));
            Parent root = (Parent) loader.load();
            stage.setScene(new Scene(root));
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
            stage.show();
            childrenGate = loader.getController();
            childrenGate.setParent(this);



        } catch (IOException e){
            e.printStackTrace();
        }
    }

    public void searchPlane(ActionEvent actionEvent) {
        try {
            Stage stage = new Stage();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/view/search_plane_form.fxml"));
            Parent root = (Parent) loader.load();
            stage.setScene(new Scene(root));
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
            stage.show();
            childrenPlane = loader.getController();
            childrenPlane.setParent(this);



        } catch (IOException e){
            e.printStackTrace();
        }
    }

    public void addNewFly(ActionEvent actionEvent) {
        Database database = new Database();
        database.addData(table,columns,  idFlight.getText() + "," + idGate.getText() +  "," + idPlane.getText() +  ", '" + dateOfFly.getValue() + "'");
        Stage stage = (Stage) dateOfFly.getScene().getWindow();
        stage.close();
    }
}
