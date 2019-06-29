package controllers;

import database.Database;
import database.Gate;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;

import java.sql.ResultSet;
import java.sql.SQLException;

public class CtrlSearchGate {
    private ObservableList<Gate> gates = FXCollections.observableArrayList();
    private  CtrlAddFly parent;
    @FXML
    public TextField gateName;
    @FXML
    public TableView<Gate> tablePlane;
    @FXML
    public TableColumn<Gate, String> columnId;
    @FXML
    public TableColumn <Gate, String> columnGateName;


    public CtrlAddFly getParent() {
        return parent;
    }

    public void setParent(CtrlAddFly parent) {
        this.parent = parent;
    }
    public void searchGate(ActionEvent actionEvent) throws SQLException {
        tablePlane.getItems().clear();
        String name = gateName.getText();
        Database db = new Database();
        ResultSet result = db.getData("public.gate","name_gate LIKE '" + name + "%'");
        while(result.next()){
            gates.add(new Gate(result.getString("id_gate"),result.getString("name_gate")));
            // nazov = result.getString("name_plane");
            // mnozstvo = result.getInt("aircraft_registration");

        }
        //planes.add(new Plane("1","Boing 737", "AR345343"));
        //planes.add(new Plane("2","Boing 757", "BL233443"));

        columnId.setCellValueFactory(new PropertyValueFactory<Gate, String>("id"));
        columnGateName.setCellValueFactory(new PropertyValueFactory<Gate, String>("name"));

        tablePlane.setItems(gates);
    }

    public void applyGate(ActionEvent actionEvent) {
        parent.setGateTextField(tablePlane.getSelectionModel().getSelectedItem().getId());
        Stage stage = (Stage) gateName.getScene().getWindow();
        stage.close();
    }
}
