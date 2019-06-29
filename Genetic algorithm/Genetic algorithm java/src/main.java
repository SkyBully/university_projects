import java.util.Scanner;

public class main {
    public static void main(String[] args) {

        int countOfGenes = 28;


        Genetics genetics = new Genetics();
        Scanner in = new Scanner(System.in);
        System.out.print("Choise selection(Turnament '1' or Rullet '2'): ");
        int typeOfSelection = in.nextInt();

        int solution = genetics.solution(countOfGenes, typeOfSelection);

    }
}
