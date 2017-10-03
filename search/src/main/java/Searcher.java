/*

index routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of iota.

*/

package black.theta;

import java.io.BufferedReader;
import java.io.StringReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Map;
import java.util.HashMap;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Date;
import java.util.Scanner;
import org.apache.lucene.util.Version;
import org.apache.lucene.analysis.util.ClasspathResourceLoader;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.en.EnglishAnalyzer;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.snowball.SnowballPorterFilterFactory;
import org.apache.lucene.analysis.tokenattributes.OffsetAttribute;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.IntPoint;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.FSDirectory;

public class Searcher {

    private static boolean DEBUG = false;
    private Searcher() {}

    private static String tokenize(Analyzer analyzer, String text) throws IOException
    {
        StringBuilder token = new StringBuilder();
        TokenStream stream = analyzer.tokenStream("content", new StringReader(text));
        Map<String, String> args = new HashMap<String, String>(){{ put("language", "English"); }};
        SnowballPorterFilterFactory factory = new SnowballPorterFilterFactory(args);
        factory.inform(new ClasspathResourceLoader());
        stream = factory.create(stream);
        OffsetAttribute offsetAttr = stream.addAttribute(OffsetAttribute.class);
        CharTermAttribute charAttr = stream.getAttribute(CharTermAttribute.class);

        try {
            stream.reset();
            while (stream.incrementToken()) {
                token.append(String.format("%s", charAttr.toString()));
                System.out.printf("token: [%s]\n", charAttr.toString());
                if (DEBUG)
                    System.out.printf("token: [%s]\n", stream.reflectAsString(true));
            }
            stream.end();
        } finally {
            stream.close();
        }

        return token.toString();
    }

    public static class Result {
        public String key;
        public String data;
        public String text;
        public double score;
        public Result(String key, String data, String text, double score) {
            this.key = key;
            this.data = data;
            this.text = text;
            this.score = score;
        }
    }

    public static Result[] search(IndexSearcher searcher, Query query, Integer maxhits) throws Exception
    {
        if (DEBUG)
            System.out.printf("query: [%s]\n", query);

        Date start = new Date();
        TopDocs docs = searcher.search(query, maxhits);
        Date end = new Date();

        if (DEBUG) {
            System.out.printf("search time: [%s]\n", end.getTime() - start.getTime());
            System.out.printf("total hits: [%d]\n", docs.totalHits);
        }

        if (docs.totalHits == 0)
            return null;

        int count = Math.min(maxhits, docs.totalHits);
        Result[] results = new Result[count];
        for (int i=0; i<count; i++) {
            ScoreDoc hit = docs.scoreDocs[i];
            Document doc = searcher.doc(hit.doc);
            results[i] = new Result(doc.get("key"), doc.get("data"), doc.get("text"), hit.score);
            if (DEBUG)
                System.out.printf(
                    "hit: [%d], score: [%f], doc.key: [%s], doc.data: [%s], doc.text: [%s]\n",
                    i, hit.score, doc.get("key"), doc.get("data"), doc.get("text")
                );
        }

        return results;
    }

    public static void search(IndexSearcher searcher, Analyzer analyzer, BufferedReader inputReader, int maxhits, int offset)
            throws Exception
    {
        QueryParser parser = new QueryParser("text", analyzer);
        int lineid = 0;
        for (String line = inputReader.readLine(); line != null; line = inputReader.readLine()) {
            try {
                ++lineid;
                Scanner scan = new Scanner(line).useDelimiter("\t");
                StringBuilder data = new StringBuilder(4096);

                for(int i = 0; i < offset; i++)
                    data.append(String.format((i == 0 ? "%s" : "\t%s"), scan.next()));

                while (scan.hasNext()) {
                    String text = scan.next();

                    if (DEBUG) {
                        System.out.printf("\nsearching... counter: [%d], maxhits: [%d], offset: [%d]\n",
                            lineid, maxhits, offset);
                        System.out.printf("input: line: [%s]\ndata: [%s]\ntext: [%s]\n",
                            line, data.toString(), text);
                    }

                    if (text.length() == 0)
                        continue;

                    Query query = parser.parse(text);
                    Result[] results = search(searcher, query, maxhits);

                    if (results != null && results.length != 0) {
                        for (int i=0; i<results.length; i++) {
                            System.out.printf("%s\t%s\t%s\t%s\t%.6f\n", data.toString(),
                                results[i].key, results[i].data, results[i].text, results[i].score
                            );
                        }
                    } else
                        System.err.printf("%s\n", data.toString());
                }
            } catch (Exception error) {
                System.out.printf("error: [%s %s]\n", error.getClass(), error.getMessage());
            }
        }
    }

    public static void main(String[] args) throws Exception
    {
        String indexuri = args[0];
        String indextype = args[1];
        int maxhits = Integer.parseInt(args[2]);
        int offset = Integer.parseInt(args[3]);
        DEBUG = Integer.parseInt(args[4]) != 0;

        IndexReader indexReader = null;
        BufferedReader inputReader = null;

        try {

            indexReader = DirectoryReader.open(FSDirectory.open(Paths.get(indexuri)));
            IndexSearcher searcher = new IndexSearcher(indexReader); Analyzer analyzer = new EnglishAnalyzer();
            inputReader = new BufferedReader(new InputStreamReader(System.in));
            search(searcher, analyzer, inputReader, maxhits, offset);

        } catch (IOException error) {
            System.out.printf("error: [%s %s]\n", error.getClass(), error.getMessage());
            throw error;
        } finally {
            if (indexReader != null)
                indexReader.close();
            if (inputReader != null)
                inputReader.close();
        }
    }
}

