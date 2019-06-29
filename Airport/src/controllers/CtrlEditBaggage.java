package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.IOException;
import java.sql.SQLException;

public class CtrlEditBaggage {
    private CtrlDeleteBaggage childrenBaggage;
    private CtrlSearchBoardingPass childrenBoardingPass;
    private CtrlAddBaggage childrenAddBoardingPass;
    @FXML
    public Label boardingPass;
    @FXML
    public Label price;

    public void setIdBoardingPassTextField(String id){
        boardingPass.setText(id);
    }

    public void setPriceTextField(String id){
        price.setText(id);
    }

    public void searchBoardingPass(ActionEvent actionEvent) {
        try {
           // Stage stage = new Stage();
            //Parent root = FXMLLoader.load(getClass().getResource("/view/search_boarding_pass_form.fxml"));
           // stage.setScene(new Scene(root));
            //stage.initModality(Modality.WINDOW_MODAL);
            //stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
           // stage.show();
            Stage stage = new Stage();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/view/search_boarding_pass_form.fxml"));
            Parent root = (Parent) loader.load();
            stage.setScene(new Scene(root));
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
            stage.show();
            childrenBoardingPass = loader.getController();
            childrenBoardingPass.setParent(this);

        } catch (IOException e){
            e.printStackTrace();
        }
    }

    public void apply(ActionEvent actionEvent) {
        Stage stage = (Stage) boardingPass.getScene().getWindow();
        stage.close();
    }

    public void addBaggage(ActionEvent actionEvent) {
        try {
            Stage stage = new Stage();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/view/add_baggage_form.fxml"));
            Parent root = (Parent) loader.load();
            stage.setScene(new Scene(root));
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
            stage.show();
            childrenAddBoardingPass = loader.getController();
            childrenAddBoardingPass.setParent(this,boardingPass.getText());
        } catch (IOException e){
            e.printStackTrace();
        }
    }

    public void deleteBaggage(ActionEvent actionEvent) throws SQLException {
        try {
            Stage stage = new Stage();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/view/delete_baggage_form.fxml"));
            Parent root = (Parent) loader.load();
            stage.setScene(new Scene(root));
            stage.initModality(Modality.WINDOW_MODAL);
            stage.initOwner(((Node)actionEvent.getSource()).getScene().getWindow());
            stage.show();
            childrenBaggage = loader.getController();
            childrenBaggage.setParent(this,boardingPass.getText());
        } catch (IOException e){
            e.printStackTrace();
        }
    }
}
