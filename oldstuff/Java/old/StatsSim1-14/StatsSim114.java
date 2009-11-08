import java.util.Random;
import tio.*;

class StatsSim {
    public static void main (String[] args) {
        double[] dist = {.335,.499,.618,.799,.872,.926,.954,.978,.985,.993,.996,1,2};
        Sim[] sims = new Sim[10];
        doSims(sims, dist);
        for(int i = 0; i < sims.length; i++) {
            System.out.println("Sim " + i + ": " + sims[i].mean + ", " + sims[i].stddev);
        }
        while(true) {
            int x = Console.in.readInt();
            char c = (char)Console.in.readCharNonwhite();
            if(c == 'h')
                printHistogram(sims, x);
            else if(c == 'r')
                printRaw(sims, x);
            else if(c == 'm')
                printMeanHist(sims);
        }
    }
    static void printHistogram(Sim[] s, int x) {
        if(x >= s.length)
            for(int i = 0; i < s.length; i++)
                printHistogram(s, i);
        else {
            System.out.println("\nSim " + x + ":");
            int m = s[x].hist[0];
            for(int i = 1; i < s[x].hist.length; i++){
                if(s[x].hist[i] > m)
                    m = s[x].hist[i];
            }
            for(int i = m; i > 0; i--){
                for(int j = 0; j < s[x].hist.length; j++){
                    if(s[x].hist[j] >= i)
                        System.out.print('*');
                    else
                        System.out.print(' ');
                }
                System.out.println();
            }
            for(int i = 0; i < s[x].hist.length; i++)
                System.out.print(i%10);
            System.out.println();
        }
    }
    static void printRaw(Sim[] s, int x){}
    static void printMeanHist(Sim[] s){}
    static void doSims(Sim[] s, double[] d) {
        for(int i = 0; i < s.length; i++) {
            s[i] = new Sim();
            s[i].rand = new Random();
            s[i].mean = 0;
            s[i].stddev = 0;
            s[i].raw = new double[20];
            s[i].hist = new int[d.length];
            for(int j = 0; j < s[i].raw.length; j++) {
                s[i].raw[j] = s[i].rand.nextDouble();
                for(int k = 0; k < d.length; k++) {
                    if(s[i].raw[j] < d[k]) {
                        s[i].hist[k]++;
                        s[i].mean+=k;
                        break;
                    }
                }
            }
            s[i].mean/=s[i].raw.length;
            for(int j = 0; j < s[i].hist.length; j++)
                s[i].stddev+=(s[i].hist[j]*((j-s[i].mean)*(j-s[i].mean)));
            s[i].stddev/=(s[i].raw.length-1);
            s[i].stddev = Math.sqrt(s[i].stddev);
        }
    }
}

class Sim {
    double[] raw;
    int[] hist;
    Random rand;
    double mean;
    double stddev;
}