public class Person {
    private int fit;
    private int[] chromosome;
    private int[][] mapa =  {{0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,-1,0,0,0,0,0,0},
                            {0,-1,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,-1,0,0,0,0,0,0,0},
                            {0,0,-1,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,-1,-1,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0}};


    public int getFit() {
        return fit;
    }

    public void setFit(int fit) {
        this.fit = fit;
    }

    public int[] getChromosome() {
        return chromosome;
    }

    public void setChromosome(int[] chromosome) {
        this.chromosome = chromosome;
    }

    public int[][] getMapa() {
        return mapa;
    }

    public void setMapa(int[][] mapa) {
        this.mapa = mapa;
    }
}
