import React, { useEffect, useState } from "react";

const FileUpload: React.FC = () => {
  const [fileContent, setFileContent] = useState<string>("");

  useEffect(() => {
    const lines = fileContent.split("\n");
    console.log(lines);
  }, [fileContent]);

  const handleFileUpload = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (file) {
      const reader = new FileReader();
      reader.onload = (e: ProgressEvent<FileReader>) => {
        const result = e.target?.result;
        if (typeof result === "string") {
          setFileContent(result);
        }
      };
      reader.readAsText(file);
    }
  };

  return (
    <div>
      <input type="file" accept=".dbc" onChange={handleFileUpload} />
      <pre>{fileContent}</pre>
    </div>
  );
};

export default FileUpload;
