package controllers;

import database.Baggage;
import database.BoardingPass;
import database.Database;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;

import java.sql.ResultSet;
import java.sql.SQLException;

public class CtrlDeleteBaggage {

    private  CtrlEditBaggage parent;
    private ObservableList<Baggage> info = FXCollections.observableArrayList();
    @FXML
    public Label boardingPass;
    @FXML
    public TableView<Baggage> tableInfo;
    @FXML
    public TableColumn<Baggage, String> columnId;
    @FXML
    public TableColumn <Baggage, String> columnWeight;

    public void delete(ActionEvent actionEvent) throws SQLException {
        System.out.println(tableInfo.getSelectionModel().getSelectedItem().getId());
        //tableInfo.getSelectionModel().getSelectedItem().getId();
        Database db = new Database();
        db.deleteDataTransaction("public.baggage","id_baggage", tableInfo.getSelectionModel().getSelectedItem().getId(),"; UPDATE public.boardingpass SET price = price - 30.00 WHERE id_boarding_pass =" + boardingPass.getText());
        ResultSet result = db.getData("public.boardingpass bp JOIN public.passenger p ON bp.id_passenger=p.id_passenger","id_boarding_pass = " + boardingPass.getText());
        while(result.next()){
            parent.setPriceTextField(result.getString("price") + ",00 EUR");
        }
        Stage stage = (Stage) boardingPass.getScene().getWindow();
        stage.close();
    }

    public void setParent(CtrlEditBaggage parent,String bp) throws SQLException {
        boardingPass.setText(bp);
        this.parent = parent;

        tableInfo.getItems().clear();
        String boardingPassText = boardingPass.getText();
        Database db = new Database();
        ResultSet result = db.getData("public.baggage","id_boarding_pass = " + boardingPassText);
        while(result.next()){
            info.add(new Baggage(result.getString("id_baggage"),result.getString("weight")));

            // nazov = result.getString("name_plane");
            // mnozstvo = result.getInt("aircraft_registration");
        }
        //planes.add(new Plane("1","Boing 737", "AR345343"));
        //planes.add(new Plane("2","Boing 757", "BL233443"));

        columnId.setCellValueFactory(new PropertyValueFactory<Baggage, String>("id"));
        columnWeight.setCellValueFactory(new PropertyValueFactory<Baggage, String>("weight"));
        tableInfo.setItems(info);
    }
}
