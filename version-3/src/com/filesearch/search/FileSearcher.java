package com.filesearch.search;

import com.filesearch.model.SearchResult;
import java.util.List;

// Interface Segregation + Dependency Inversion:
// যেকোনো search strategy এই interface implement করতে পারবে
public interface FileSearcher {
    List<SearchResult> search(String rootPath, String query);
}
