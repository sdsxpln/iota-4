/*
index routines
Copyright (C) 2013-2017 Roman Fakhrazeyev <xinoir@theta.black> 
This file is part of iota.
*/

package black.theta;

import java.io.Reader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.Date;
import java.util.Scanner;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.analysis.en.EnglishAnalyzer;
import org.apache.lucene.document.LongPoint;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.document.IntPoint;
import org.apache.lucene.document.StoredField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.Term;
import org.apache.lucene.analysis.AnalyzerWrapper;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.core.TypeTokenFilter;
import org.apache.lucene.analysis.en.PorterStemFilter;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

public class Indexer {

    private Indexer() {}

    static void createIndex(IndexWriter writer, Path path, long timestamp) throws IOException 
    {
        try (InputStream stream = Files.newInputStream(path)) {
            Document doc = new Document();
            doc.add(new StringField("path", path.toString(), Field.Store.YES));
            doc.add(new LongPoint("timestamp", timestamp));
            doc.add(new TextField("contents", new BufferedReader(
                new InputStreamReader(stream, StandardCharsets.UTF_8))));

            if (writer.getConfig().getOpenMode() == OpenMode.CREATE)
                writer.addDocument(doc);
            else
                writer.updateDocument(new Term("path", path.toString()), doc);
        }
    }

    static void createIndex(final IndexWriter writer, Path path) throws IOException
    {
        if (Files.isDirectory(path)) {
            Files.walkFileTree(path, new SimpleFileVisitor<Path>() {

                @Override
                public FileVisitResult visitFile(Path path, BasicFileAttributes attrs) throws IOException
                {
                    try {
                        createIndex(writer, path, attrs.lastModifiedTime().toMillis());
                    } catch(Exception error) {
                        System.out.printf("error: [%s %s]\n", error.getClass(), error.getMessage());
                    }
                    return FileVisitResult.CONTINUE;
                }

            });
        } else {
            createIndex(writer, path, Files.getLastModifiedTime(path).toMillis());
        }
    }

    public static void index(IndexWriter indexWriter, BufferedReader inputReader) throws Exception
    {
        for (String line = inputReader.readLine(); line != null; line = inputReader.readLine()) {
            Scanner scan = new Scanner(line).useDelimiter("\t");
            Document doc = new Document();
            String key = scan.next();
            String data = scan.next();
            String text = scan.next();
            System.err.printf("indexing: key: [%s], data: [%s], text: [%s]\n", key, data, text);
            doc.add(new Field("key", key, StringField.TYPE_STORED));
            doc.add(new Field("data", data, StringField.TYPE_STORED));
            doc.add(new Field("text", text, TextField.TYPE_STORED));
            indexWriter.addDocument(doc);
        }
    }

    public static void main(String[] args) throws Exception
    {
        final int INDEX_RAM_BUFFER_SIZE = 4096;
        String indexuri = args[0];
        String indextype= args[1];
        Date start = new Date();
        IndexWriter indexWriter = null;
        BufferedReader inputReader = null;

        try {

            Directory indexpath = FSDirectory.open(Paths.get(indexuri));
            IndexWriterConfig indexconfig = new IndexWriterConfig(new EnglishAnalyzer());
            indexconfig.setOpenMode(OpenMode.CREATE_OR_APPEND);
            indexconfig.setRAMBufferSizeMB(INDEX_RAM_BUFFER_SIZE);
            indexWriter = new IndexWriter(indexpath, indexconfig);
            inputReader = new BufferedReader(new InputStreamReader(System.in));
            index(indexWriter, inputReader);

        } catch (IOException error) {
            System.out.printf("error: [%s %s]\n", error.getClass(), error.getMessage());
            throw error;
        } finally {
            if (indexWriter != null) {
                try {
                    indexWriter.forceMerge(1);
                    indexWriter.close();
                } catch(Exception error) {
                    System.out.printf("error: [%s %s]\n", error.getClass(), error.getMessage());
                }
            }

            if (inputReader != null)
                inputReader.close();
        }

        Date end = new Date();
        System.out.printf("created indexes: [%d ms]\n", end.getTime() - start.getTime());
    }
}

