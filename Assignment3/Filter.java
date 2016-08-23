import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Filter {
	
	static int spamWords;
	static int hamWords;
	static HashMap<String,Integer> masterSpamCount = new HashMap<String,Integer>();
	static HashMap<String,Integer> masterHamCount = new HashMap<String,Integer>();
	static HashMap<String,Integer> spamCount = new HashMap<String,Integer>();
	static HashMap<String,Integer> hamCount = new HashMap<String,Integer>();
	static Set<String> testWords = new HashSet<String>();

	//reads in file directory, iterates through files and adds word frequencies to spam or ham hash map.
	public static void train_files(File dir, boolean isSpam) throws IOException{
		File[] directoryListing = dir.listFiles();
		String word;
		
	    if (directoryListing != null && isSpam) {
	    	for (File child : directoryListing) {
	    		FileReader fr = new FileReader(child.getPath());
	    		Scanner in = new Scanner(fr);
	    		while (in.hasNext()){
	    			word = in.next();
	    			if (masterSpamCount.containsKey(word)){
	    				int val = masterSpamCount.get(word);
	    				masterSpamCount.put(word, val+1);
	    			}
	    			else masterSpamCount.put(word, 1);
	    		}
	    		fr.close();
	    	}
	    }
	    else{
	    	for (File child : directoryListing) {
	    		FileReader fr = new FileReader(child.getPath());
	    		Scanner in = new Scanner(fr);
	    		while (in.hasNext()){
	    			word = in.next().toLowerCase();
	    			if (masterHamCount.containsKey(word)){
	    				int val = masterHamCount.get(word);
	    				masterHamCount.put(word, val+1);
	    			}
	    			else masterHamCount.put(word, 1);
	    		}
	    		fr.close();
	    	}
	    }
	}
	
	//creates new lexicon based on words that appeared more than k times.
	public static void newLexicon(int k){
		spamCount.clear();
		hamCount.clear();
		spamWords = 0;
		hamWords = 0;
		
		int val1, val2;
		
		for (String word : masterSpamCount.keySet()){
			val1 = masterSpamCount.get(word);
			if (masterHamCount.containsKey(word)){
				val2 = masterHamCount.get(word);
			}
			else val2 = 0;
			
			if (val1+val2 > k){
				spamCount.put(word, val1);
				if (val2 > 0){
					hamCount.put(word, val2);
				}
				spamWords += val1;
				hamWords += val2;
			}
		}
		
		for (String word : masterHamCount.keySet()){
			val2 = masterHamCount.get(word);
			if (!masterSpamCount.containsKey(word)){
				if (val2 > k){
					hamCount.put(word, val2);
					hamWords += val2;
				}
			}
		}
	}
	
	
	
	//Function that displays conditional word probabilites
	public static void print_elems(int k){
		for (String key : spamCount.keySet()){
			if (spamCount.get(key) > k){
				if (!hamCount.containsKey(key)){
					System.out.printf("Word: %s ---> Spam: %d --- Ham: 0\n", key, spamCount.get(key));
					System.out.printf("Spam: %.5f --- Ham: 0.00000\n",(double)spamCount.get(key)/(double)spamWords);
				}
				else {
					System.out.printf("Word: %s ---> Spam: %d --- Ham: %d\n", key, spamCount.get(key),hamCount.get(key));
					System.out.printf("Spam: %.5f --- Ham: %.5f\n",(double)spamCount.get(key)/(double)spamWords, (double)hamCount.get(key)/(double)hamWords);
				}

			}
		}
	}
	
	//return vector of words in email from a "test" case
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
		
		for (int i = 0; i < words.size(); i++){
			String word = words.get(i);
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
		newLexicon(k);
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
		train_files(new File(args[0]),true);
		train_files(new File(args[1]),false);
		
		int k = Integer.parseInt(args[4]);
		int m = Integer.parseInt(args[5]);
		System.out.printf("k = %d,  m = %d\n",k,m);
		System.out.printf("Spam: %d percent accurate\n",driver(new File(args[2]),k,m));
		System.out.printf("Ham: %d percent accurate\n",100-driver(new File(args[3]),k,m));
	}
}
