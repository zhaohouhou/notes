
# **Solr查询**

## 1. Solr常用查询参数

本节包含Solr的通用查询参数，Search RequestHandlers 支持这些参数。

### defType 参数

defType 参数选择 Solr 应该用来处理请求中的主查询参数（q）的查询解析器。例如：

    defType=dismax

如果没有指定 defType 参数，则默认使用标准查询解析器(The Standard Query Parser)。（如：`defType=lucene`）

### sort 参数

sort 参数按升序 (asc) 或降序 (desc) 顺序排列搜索结果。方向可以全部以小写字母或全部大写字母输入（即，asc 或者ASC）。

- 排序顺序必须包含一个字段名称（或作为伪字段的 score），后跟空格（在 URL 字符串中转义为 + 或 %20），然后是排序方向（asc 或 desc）。

- 多个排序顺序可以用逗号隔开，使用下面的语法：

      sort=<field name><direction>,<field name><direction>],…​

  如果提供了多个排序标准，则只有在第一个条目产生并列时才使用第二个条目。以此类推。


### **<font color=red>start 参数</font>**

start 参数指定查询结果集合的偏移量，并指示 Solr 从此偏移量开始显示结果。默认值为 0。

当 start 参数不为0， Solr 跳过前面的记录而从由偏移量标识的文档开始。因此可使用 start 参数来进行分页显示。

### rows 参数

可以使用 rows 参数将查询的结果分页。该参数指定 Solr 一次返回的最大文档数目。

<font color=#6495ED>rows 默认值是10，也就是说默认情况下 Solr 一次返回 10 个文档响应查询。</font>

### fq (Filter Query) 参数

fq 参数定义了一个查询，可以用来限制可以返回的文档的超集，而不影响 score。对于加快复杂查询非常有用，因为指定的查询 fq 是独立于主查询而被缓存的。当以后的查询使用相同的过滤器时，会有一个缓存命中，过滤器结果从缓存中快速返回。

### fl（Field List）参数

fl 参数限定了查询返回信息的字段。这些字段必须是`stored="true"`或 `docValues="true"`。

字段列表可以为空格分隔或逗号分隔。通配符 * 选择文档中的所有字段。还可以添加伪字段（pseudo-fields）、函数和 transformers。示例：

```
# Functions
fl=id,title,product(price,popularity)

# Document Transformers
fl=id,title,[explain]

# Field Name Aliases
fl=id,sales_price:price,secret_sauce:prod(price,popularity),why_score:[explain style=nl]
```

### debug 参数

- `debug=query`：仅返回有关查询的调试信息。
- `debug=timing`：返回有关查询花费多长时间处理的调试信息。
- `debug=results`：返回关于 score 结果的调试信息。
- `debug=all`（`debug=true`）：返回关于 request 请求的所有可用调试信息。

### 其他

<table>
<tr>
<td>**explainOther 参数**</td>
<td>指定了一个 Lucene 查询来标识一组文档。如果此参数设置为非空值，则查询将返回调试信息以及与 Lucene 查询相匹配的每个文档的“说明信息”</td>
</tr>
<tr>
<td>**timeAllowed 参数**</td>
<td>指定允许搜索完成的时间量（以毫秒为单位）</td>
</tr>
<tr>
<td>**segmentTerminateEarly 参数**</td>
<td>merge policy相关参数</td>
</tr>
<tr>
<td>**omitHeader 参数**</td>
<td>默认为false。若设为true，则返回结果中不含header。header包含有关请求的信息，例如完成所耗时间。</td>
</tr>
<tr>
<td>**wt 参数**</td>
<td>选择 Solr 用来格式化查询响应的 Response Writer。默认为JSON格式</td>
</tr>
<tr>
<td>**cache 参数**</td>
<td>Solr 默认缓存所有查询结果。要禁用结果缓存，需设 `cache=false`。</td>
</tr>
<tr>
<td>**logParamsList 参数**</td>
<td>Solr 默认记录所有请求参数。logParamsList参数用来限制哪些参数需要日志记录。例如`logParamsList=q,fq`表示只有'q'和'fq'参数会被记录。如果不想记录参数，可令logParamsList 为空。该参数适用于的任何类型的 Solr 请求。</td>
</tr>
<tr>
<td>**echoParams 参数**</td>
<td>控制响应header中包含的请求参数信息。</td>
</tr>
</table>

## 2. JSON Request API

下面的示例http请求只使用了查询参数：

    curl http://localhost:8983/solr/COLLECTION_NAME/query?q=name:123456

其中`name`为查询的字段名称`123456`为查询关键字。
Solr对该请求返回JSON格式的响应数据：

```
{
  "responseHeader":{
    "params":{
      "q":"name:123456"},
      ...
    },
  "response":{"numFound":...,"start":...,"maxScore":...,"docs":[
      {
        "name":"123456",
        ...
        }]
  }}
```

通过JSON Request API可以传送一个完整的JSON body，作为查询请求。
上面的请求可写为：

    curl http://localhost:8983/solr/COLLECTION_NAME/query -d 'json={"name":"123456"}'

同一个请求中的多个json参数会被合并。同一种参数，后面的值会覆盖前面的。
json参数先于其他请求参数被处理，因此其他参数会覆盖json body中的同一参数。

一些标准查询参数具有JSON等价字段：

<table class="tableblock frame-all grid-all spread">
<caption class="title">Standard query parameters to JSON field</caption>
<colgroup>
<col style="width: 50%;">
<col style="width: 50%;">
</colgroup>
<thead>
<tr>
<th class="tableblock halign-left valign-top">Query parameters</th>
<th class="tableblock halign-left valign-top">JSON field equivalent</th>
</tr>
</thead>
<tbody>
<tr>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>q</code></p></td>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>query</code></p></td>
</tr>
<tr>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>fq</code></p></td>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>filter</code></p></td>
</tr>
<tr>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>offset</code></p></td>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>start</code></p></td>
</tr>
<tr>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>limit</code></p></td>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>rows</code></p></td>
</tr>
<tr>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>sort</code></p></td>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>sort</code></p></td>
</tr>
<tr>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>json.facet</code></p></td>
<td class="tableblock halign-left valign-top"><p class="tableblock"><code>facet</code></p></td>
</tr>
</tbody>
</table>


## ref:

http://lucene.apache.org/solr/guide/7_1/searching.html

https://www.w3cschool.cn/solr_doc/solr_doc-vyp82ghf.html

</br></br>
