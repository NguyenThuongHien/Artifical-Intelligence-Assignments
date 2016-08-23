import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Trigram {
	
	static int spamWords;
	static int hamWords;
	static HashMap<String,Integer> spamCount = new HashMap<String,Integer>();
	static HashMap<String,Integer> hamCount = new HashMap<String,Integer>();
	static Set<String> testWords = new HashSet<String>();
 	
	//reads in file directory, iterates through files and adds trigram frequencies to spam or ham hash map.
	public static void trigramTraining(File dir, boolean isSpam) throws IOException{
		File[] directoryListing = dir.listFiles();
		
	    if (directoryListing != null && isSpam) {
	    	for (File child : directoryListing) {
	    		ArrayList<String> words = getTestData(child);
	    		
	    		for (int i = 0; i < words.size()-2; i++){
	    			String trigram = words.get(i)+words.get(i+1)+words.get(i+2);
	    			spamWords++;
	    			if (!spamCount.containsKey(trigram)){
	    				spamCount.put(trigram, 1);
	    			}
	    			else{
	    				int val = spamCount.get(trigram);
	    				spamCount.put(trigram, val+1);
	    			}
	    		}
	    	}
	    }
	    			    
	    else{
	    	for (File child : directoryListing) {
	    		ArrayList<String> words = getTestData(child);
	    		
	    		for (int i = 0; i < words.size()-2; i++){
	    			String trigram = words.get(i)+words.get(i+1)+words.get(i+2);
	    			hamWords++;
	    			if (!hamCount.containsKey(trigram)){
	    				hamCount.put(trigram, 1);
	    			}
	    			else{
	    				int val = hamCount.get(trigram);
	    				hamCount.put(trigram, val+1);
	    			}
	    		}
	    	}
	    }
	}

	//return vector of trigrams in email from a "test" case
	public static ArrayList<String> getTestData(File child) throws IOException{
		ArrayList<String> words = new ArrayList<String>();
	    
	    FileReader fr = new FileReader(child.getPath());
	    Scanner in = new Scanner(fr);
		while (in.hasNext()){
			words.add(in.next());
		}
		fr.close();
		return words;
	}
	    
	//classify email as spam (true) or ham (false) using Naive Bayes and Laplacian smoothing
	public static boolean isProbableSpam(ArrayList<String> words, int k, int m){
		double prob1 = Math.log(0.5);//spam
		double prob2 = Math.log(0.5);//ham
		int V1 = spamCount.size();
		int V2 = hamCount.size();
		int val1,val2;
		double wordProb1, wordProb2;
		
		for (int i = 0; i < words.size()-2; i++){
			String word = words.get(i)+words.get(i+1)+words.get(i+2);
			if (!spamCount.containsKey(word)){
				val1 = 0;
			}
			else val1 = spamCount.get(word);
			if (!hamCount.containsKey(word)){
				val2 = 0;
			}
			else val2 = hamCount.get(word);
			
			wordProb1 = (double)(val1+m) / (double) (spamWords + m*V1);
			wordProb2 = (double)(val2+m) / (double) (hamWords + m*V2);
			prob1 += Math.log(wordProb1);
			prob2 += Math.log(wordProb2);
		}
		
		if (prob1 > prob2) return true;
		return false;
	}
	
	//reads in directory of "test" emails, creates new lexicon, and counts # of spam/ham (returns spam)
	public static int driver(File dir, int k, int m) throws IOException{
		File[] directoryListing = dir.listFiles();
		int spam = 0;
		int ham = 0;
		
		for (File child : directoryListing){
			ArrayList<String> emailData = getTestData(child);
			if (isProbableSpam(emailData,k,m)){
				spam++;
			}
			else {
				ham++;
			}
		}
		return spam;
	}
	
	public static void main(String[] args) throws IOException {
		Scanner scanner = new Scanner(System.in);
		trigramTraining(new File(args[0]),true);
		trigramTraining(new File(args[1]),false);
		
		int k = Integer.parseInt(args[4]);
		int m = Integer.parseInt(args[5]);
		System.out.printf("k = %d,  m = %d\n",k,m);
		System.out.printf("Spam: %d percent accurate\n",driver(new File(args[2]),k,m));
		System.out.printf("Ham: %d percent accurate\n",100-driver(new File(args[3]),k,m));
	}
}

