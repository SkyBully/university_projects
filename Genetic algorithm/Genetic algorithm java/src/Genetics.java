import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;

public class Genetics {


    public int solution(int countOfGenes, int typeOfSelection) {
        //firstGeneration(countOfGenes);
        LinkedList<Person> listOfPerson = firstGeneration(countOfGenes);
        for (int i = 0; i < 500; i++) {// 500 generacij

            LinkedList<Person> selectedPersons = new LinkedList<>();
            if (typeOfSelection == 1) {selectedPersons = selection(listOfPerson);}
            else {selectedPersons = selection2(listOfPerson);}

            listOfPerson = crossing(selectedPersons);
            //int avgFit = 0;
            //int maxFit = 0;
            //for (Person tmp:listOfPerson) {
            //    avgFit += tmp.getFit();
            //    if(tmp.getFit() > maxFit)
            //        maxFit = tmp.getFit();
            //}
            //avgFit /= listOfPerson.size();
            //System.out.println(avgFit + "  " + maxFit);
            //System.out.println("LOL" + listOfPerson.get(0).getFit());
            //System.out.println("generacia = " + i);
        }
        return 0;
    }


    //prva generacia
    public LinkedList<Person> firstGeneration(int countOfGenes) {
        LinkedList<Person> generation1 = new LinkedList<Person>();
        for (int i = 0; i < 100; i++) {//100 jedincov
            Person person = new Person();
            int[] newChromosome = new int[countOfGenes];
            for (int j = 0; j < countOfGenes; j++) {//generacia novej chromozomy
                newChromosome[j] = (int) (Math.random() * countOfGenes);
                //System.out.println(newChromosome[j]);
            }
            person.setChromosome(newChromosome);
            //System.out.println("--- " +person.getMapa()[2][1]);
            fitness(person);//vypocet fitness
            generation1.add(person);
        }
        generation1 = sortGeneration(generation1);//usporiadanie podla fitness
        return generation1;
    }

    //usporiada zoznam podla fitness
    public LinkedList<Person> sortGeneration(LinkedList<Person> generation) {
        LinkedList<Person> sotredGeneration = new LinkedList<>();
        Person tmpPerson = null;

        for (int i = 0; i < 100/*generation.size()*/; i++) {
            int tmp = 0;
            for (Person p : generation) {
                if (p.getFit() > tmp) {
                    tmp = p.getFit();
                    tmpPerson = p;
                }
            }
            sotredGeneration.add(tmpPerson);
            generation.remove(tmpPerson);

        }
        return sotredGeneration;
    }

    //selekcia turnajom
    public LinkedList<Person> selection(LinkedList<Person> generation) {

        LinkedList<Person> selectedPersoms = new LinkedList<>();
        for (int i = 0; i < 80; i++) {//70
            int index1 = (int) (Math.random() * (100 - i));
            int index2 = (int) (Math.random() * (100 - i));

            int fr1 = generation.get(index1).getFit();
            int fr2 = generation.get(index2).getFit();

            if (fr1 > fr2) {
                selectedPersoms.add(generation.get(index1));
                generation.remove(index1);

            } else {
                selectedPersoms.add(generation.get(index2));
                generation.remove(index2);
            }
        }
        selectedPersoms = sortGeneration(selectedPersoms);
        return selectedPersoms;

    }

    //selekcia rullet
    public LinkedList<Person> selection2 (LinkedList<Person> generation){

        LinkedList<Person> selectedPersoms = new LinkedList<>();
        for (int i = 0; i < 80; i++) {
            for (int j = 0; j < 100; j++) {
                int r = (int) (Math.random() * 5050);

                int tmp = 0;
                int count = 0;
                    tmp += (100 - j);
                    //System.out.println("tmp = " + tmp);
                    //System.out.println("R = " + r);
                    count++;
                selectedPersoms.add(generation.get(count));
            }
        }
        selectedPersoms = sortGeneration(selectedPersoms);
        return selectedPersoms;
    }

    // restartovanie mapy je potrebne ked jedinec zo starej generacii prechadza v novu
    public void resetMap(Person person) {
        person.setFit(0);
        int[][] newMap = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0},
                {0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        person.setMapa(newMap);
    }

    //krizenie
    public LinkedList<Person> crossing(LinkedList<Person> selectedGeneration) {

        LinkedList<Person> newGeneration = new LinkedList<Person>();
        int index1 = 0;
        int index2 = 1;
        int length = 28;//selectedGeneration.get(1).getChromosome().length - 1;

        for (int i = 0; i < 40; i++) {//35
            Person person = new Person();
            int[] newChromosome1 = new int[length];
            int[] newChromosome2 = new int[length];
            int[] chromosome1 = new int[length];
            int[] chromosome2 = new int[length];

            if(i > 1 && i < 21)// jedince zo starej generacii
            {
                resetMap(selectedGeneration.get(i));
                newGeneration.add(selectedGeneration.get(i));
            }

            /*int r = (int) (Math.random() * 100);
            resetMap(selectedGeneration.get(r));
            newGeneration.add(selectedGeneration.get(r));*/


            chromosome1 = selectedGeneration.get(index1).getChromosome();
            chromosome2 = selectedGeneration.get(index2).getChromosome();

            int tmpRandom = (int) (Math.random() * 28);
            for (int j = 0; j < length; j++) {
                if (j <= /*20*/tmpRandom) {
                    newChromosome1[j] = chromosome1[j];
                } else newChromosome1[j] = chromosome2[j];
            }
            int r = (int) (Math.random() * 10);
            if (r < 3)
                mutation(newChromosome1);

            person.setChromosome(newChromosome1);
            fitness(person);
            newGeneration.add(person);

            person = new Person();
            for (int j = 0; j < length; j++) {
                if (j <= /*20*/tmpRandom) {
                    newChromosome2[j] = chromosome2[j];
                } else newChromosome2[j] = chromosome1[j];
            }
            r = (int) (Math.random() * 10);
            if (r < 3)
                mutation(newChromosome2);
            person.setChromosome(newChromosome2);
            fitness(person);
            newGeneration.add(person);


            index1 += 2;//!!!!!!!!!!!!!!
            index2 += 2;
            //System.out.println("index1 = " + index1 + "index2 = " + index2);

        }
        newGeneration = sortGeneration(newGeneration);
        return newGeneration;
    }


    public void mutation(int[] chromosome) {
        int r = (int) (Math.random() * chromosome.length);
        for (int i = 0; i < r; i++) {
            int tmp = (int) (Math.random() * chromosome.length);
            //chromosome[tmp]++;
            chromosome[tmp] += 3;
            chromosome[tmp] %= chromosome.length;
        }
    }

    public void fitness(Person person) {
        makeNewMap(person);
        //person.setMapa(makeNewMap(person));
        int countFitness = 0;
        for (int i = 0; i < person.getMapa().length; i++) {
            for (int j = 0; j < person.getMapa()[0].length; j++) {
                if (person.getMapa()[i][j] > 0) {

                    countFitness++;
                }
            }
        }
        //System.out.println(countFitness);
        person.setFit(countFitness);
        if (countFitness == 114 /*person.getMapa().length * person.getMapa()[0].length - 6*/) {//6 заменить на кл-во камней
            //это решение
            System.out.printf("Chromosome: ");
            for (int i = 0; i < 28; i++) {
                System.out.printf("%d ", person.getChromosome()[i]);
            }
            System.out.println();
            for (int i = 0; i < person.getMapa().length; i++) {
                for (int j = 0; j < person.getMapa()[0].length; j++) {
                    System.out.printf("%3d ", person.getMapa()[i][j]);
                }
                System.out.printf("\n");
            }
            System.exit(0);
        }
    }


    //public void makeNewMap(Person person) {
    public void makeNewMap(Person person) {
        int X = 1, Y = 2;
        int[] chromosome = person.getChromosome();
        //int[] chromosome = {9, 6, 26, 18, 16, 17, 17, 25, 15, 6, 14, 1, 17, 2, 6, 21, 17, 5, 27, 25, 2, 13, 15, 27, 24, 10, 12, 8};
        //int[][] newMap = person.getMapa();//   ????????
        /*for (int i = 0; i < 28; i++) {
            System.out.printf("%d ", chromosome[i]);
        }
        System.out.println();*/


//        if((chromosome[0] % 2 == 0) && (chromosome[0] < chromosome.length/2))
//            X = 0;
//        else if((chromosome[0] % 2 == 0) && (chromosome[0] >= chromosome.length/2))
//            X = chromosome.length - 1;
//        else X = chromosome[0];
//
//        if((chromosome[1] % 2 != 0) && (chromosome[1] < chromosome.length/2))
//            Y = 0;
//        else if((chromosome[1] % 2 != 0) && (chromosome[1] >= chromosome.length/2))
//            Y = chromosome.length - 1;
//        else Y = chromosome[1];


//        if (((chromosome[0] % 2 == 0) && (chromosome[0] < chromosome.length / 2)) || ((chromosome[0] % 2 == 0) && (chromosome[0] >= chromosome.length / 2))) {
//            if ((chromosome[0] % 2 == 0) && (chromosome[0] < chromosome.length / 2))
//                X = 0;
//            else if ((chromosome[0] % 2 == 0) && (chromosome[0] >= chromosome.length / 2))
//                X = newMap[0].length - 1;
//            Y = chromosome[1];
//            Y %= 10;
//            X %= 12;
//
//            System.out.println("v IF x = " + X);
//            System.out.println("v IF Y = " + Y);
//        } else {
//
//            X = chromosome[0];
//            X %= 12;
//            if (/*(chromosome[1] % 2 != 0) &&*/ (chromosome[1] < chromosome.length / 2) && X != 0)
//                Y = 0;
//            else if (/*(chromosome[1] % 2 != 0) &&*/ (chromosome[1] >= chromosome.length / 2) && X != 0)
//                Y = newMap.length - 1;
//            Y %= 12;
//            System.out.println("v ELSE x = " + X);
//            System.out.println("v ELSE Y = " + Y);
//        }

        //int instruction = 0;
        int num = 0;
        while (true) {

            int count = 0;
            boolean check = false;
            int instruction = 0;
            while (person.getMapa()[Y][X] != 0) {

                count++;

                if (count == 58) {
                    //System.out.println("**********************************");
                    check = true;
                    break;
                }
                //////////////////////////////////////////////////////////////////
                if(chromosome[instruction] <= 13){
                    if(chromosome[instruction] % 2 == 0){
                        X = 0;
                    }
                    else {
                        X = 11;
                    }
                    instruction++;
                    instruction %= 28;
                    Y = ( chromosome[instruction]+count) % 10;
                }
                else {
                    if(chromosome[instruction] % 2 == 0){
                        Y = 0;
                    }
                    else {
                        Y = 9;
                    }
                    instruction++;
                    instruction %= 28;
                    X = ( chromosome[instruction]+count) % 12;
                }
                instruction++;
                instruction %= 28;
                //////////////////////////////////////////////////////////////////

//                if (((person.getChromosome()[instruction] % 2 == 0) /*&& (person.getChromosome()[instruction] < person.getChromosome().length / 2)) ||
//                        ((person.getChromosome()[instruction] % 2 == 0) && (person.getChromosome()[instruction] >= person.getChromosome().length / 2)*/)) {
//
//                    if (/*(person.getChromosome()[instruction] % 2 == 0) && */(person.getChromosome()[instruction] < person.getChromosome().length / 2))
//                        X = 0;
//                    else if (/*(person.getChromosome()[instruction] % 2 == 0) && */(person.getChromosome()[instruction] >= person.getChromosome().length / 2))
//                        X = person.getMapa()[0].length - 1;
//                    Y = person.getChromosome()[instruction + 1];
//                    Y %= 10;
//                    //X %= 12;
//
//                    //System.out.println("v IF x = " + X);
//                    //System.out.println("v IF Y = " + Y);
//                } else {
//
//                    X = person.getChromosome()[instruction];
//                    X %= 12;
//                    if (/*(person.getChromosome()[instruction + 1] % 2 != 0) && */(person.getChromosome()[instruction + 1] < person.getChromosome().length / 2) && X != 0)
//                        Y = 0;
//                    else if (/*(person.getChromosome()[instruction + 1] % 2 != 0) && */(person.getChromosome()[instruction + 1] >= person.getChromosome().length / 2) && X != 0)
//                        Y = person.getMapa().length - 1;
//                    //Y %= 10;
//                    //System.out.println("v ELSE x = " + X);
//                    //System.out.println("v ELSE Y = " + Y);
//                }
//                instruction++;
//                //instruction += 2;
//                instruction %= 27;
            }


            if (check)
                break;

            if (X == 0) {
                //System.out.println(" ---  " + num);
                num++;
                if (makeRoad(person.getMapa(), X, Y, 1, num) == 0)
                    break;
//                System.out.println("x = " + X);
//                System.out.println("Y = " + Y);
            } else if (X == 11) {
                num++;
                if (makeRoad(person.getMapa(), X, Y, 2, num) == 0)
                    break;
            } else if (Y == 0) {
                num++;
                if (makeRoad(person.getMapa(), X, Y, 3, num) == 0)
                    break;
            } else if (Y == 9) {
                num++;
                if (makeRoad(person.getMapa(), X, Y, 4, num) == 0)
                    break;
            }


        }
        //person.setMapa(newMap);
    }


    private int makeRoad(int[][] mapa, int x, int y, int course, int num) {
        //System.out.println("lol");
        boolean inMap = true;
        while (x < mapa[0].length && x >= 0 && y < mapa.length && y >= 0) {
            //слева на право
            if (course == 1) {
                while (x < mapa[0].length) {
                    //System.out.println("### x = " + x);
                    if (mapa[y][x] != 0) {
                        x--;
                        if (y + 1 <= 9 && mapa[y + 1][x] == 0) {
                            course = 3;
                            y++;
                        } else if (y - 1 >= 0 && mapa[y - 1][x] == 0) {
                            course = 4;
                            y--;
                        } else if (/*mapa[y + 1][x] == mapa.length || mapa[y - 1][x] == 0*/y + 1 >/*=*/ 9 || y - 1 </*=*/ 0)
                            inMap = false;
                        else course = 0;

                        break;
                    }
                    //System.out.println("course == 1");
                    //System.out.println("X = " + x);
                    //System.out.println("Y = " + y);
                    mapa[y][x] = num;
                    x++;
                }
            }

            //справа на лево
            if (course == 2) {
                while (x >= 0) {
                    if (mapa[y][x] != 0) {
                        x++;
                        if (y - 1 >= 0 && mapa[y - 1][x] == 0) {
                            course = 4;
                            y--;
                        } else if (y + 1 <= 9 && mapa[y + 1][x] == 0) {
                            course = 3;
                            y++;
                        } else if (/*mapa[y + 1][x] == mapa.length || mapa[y - 1][x] == 0*/y + 1 >/*=*/ 9 || y - 1 </*=*/ 0)
                            inMap = false;
                        else course = 0;
                        break;
                    }
                    //System.out.println("course == 2");
                    //System.out.println("X = " + x);
                    //System.out.println("Y = " + y);
                    mapa[y][x] = num;
                    x--;
                }
            }

            //сверху вниз
            if (course == 3) {
                while (y < mapa.length) {
                    if (mapa[y][x] != 0) {
                        y--;
                        if (x - 1 >= 0 && mapa[y][x - 1] == 0) {
                            course = 2;
                            x--;
                        } else if (x + 1 <= 11 && mapa[y][x + 1] == 0) {//!!!!!!!!
                            course = 1;
                            x++;
                        } else if (/*mapa[y][x + 1] == mapa[0].length || mapa[y][x - 1] == 0*/x + 1 >/*=*/ 11 || x - 1 </*=*/ 0)
                            inMap = false;
                        else course = 0;
                        break;
                    }
                    //System.out.println("course == 3");
                    //System.out.println("X = " + x);
                    //System.out.println("Y = " + y);
                    mapa[y][x] = num;
                    y++;
                }
            }

            //снизу вверх
            if (course == 4) {
                while (y >= 0) {
                    if (mapa[y][x] != 0) {
                        y++;
                        if (x + 1 <= 11 && mapa[y][x + 1] == 0) {
                            course = 1;
                            x++;
                        } else if (x - 1 >= 0 && mapa[y][x - 1] == 0) {
                            course = 2;
                            x--;
                        } else if (/*mapa[y][x + 1] == mapa[0].length || mapa[y][x - 1] == 0*/x + 1 >/*=*/ 11 || x - 1 </*=*/ 0)
                            inMap = false;
                        else course = 0;
                        break;
                    }
                    //System.out.println("course == 4");
                    //System.out.println("X = " + x);
                    //System.out.println("Y = " + y);
                    mapa[y][x] = num;
                    y--;
                }
            }

            if (course == 0) {// ak v mrtvom bode
                /*for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 12; j++) {
                        System.out.printf("%d",mapa[i][j]);
                    }
                    System.out.println();
                }
                System.out.println();*/
                return 0;
            }

            if (inMap == false) //если некуда идти
                break;

        }

        /*for (int i = 0; i < mapa.length; i++) {
            for (int j = 0; j < mapa[0].length; j++) {
                System.out.printf("%3d ", mapa[i][j]);
            }
            System.out.printf("\n");
        }*/
        /*for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 12; j++) {
                System.out.printf("%d",mapa[i][j]);
            }
            System.out.println();
        }
        System.out.println();*/

        return 1;
    }


}
