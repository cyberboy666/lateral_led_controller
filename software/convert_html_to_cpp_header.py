def read_html_file(html_file_path):
    with open(html_file_path, 'r') as file:
        return file.read().replace('\n', '')  # Replace newlines with empty string

def write_cpp_header(html_content, output_file_path):
    escaped_content = html_content.replace('"', '\\"')  # Escape quotes in HTML content
    cpp_header_content = f"""\
    #ifndef HTML_CONTENT_H
    #define HTML_CONTENT_H

    const char* index_html PROGMEM = "{escaped_content}";

    #endif
    """

    with open(output_file_path, 'w') as file:
        file.write(cpp_header_content)

if __name__ == "__main__":
    html_file_path = 'index.html'  # Path to your HTML file
    output_file_path = 'src/html_content.h'  # Output CPP header file

    html_content = read_html_file(html_file_path)
    write_cpp_header(html_content, output_file_path)

    print(f"HTML content from '{html_file_path}' converted and saved to '{output_file_path}'")
