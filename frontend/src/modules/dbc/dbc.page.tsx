import ProgressBar from "./progressbar";
import FileList from "./fileList";
import { useState } from "react";

function Dbc() {
  const files = ["093-134.dbc", "EmoticomV3-5.dbc", "MartinUAV_version4.424.dbc"];
  const storage = { usedSpaceKB: 841, totalSpaceKB: 1245 };

  return (
    <div className="flex flex-col gap-10">
      <span>DBC Settings</span>
      <div className="flex flex-col gap-1">
        <span className="text-start text-lg text-gray-300 w-full">Free Space:</span>
        <ProgressBar used={storage.usedSpaceKB} total={storage.totalSpaceKB} />
      </div>
      <button className="bg-purple-600 hover:bg-purple-700 active:bg-purple-800 rounded-xl p-2">
        Add new DBC-File
      </button>
      <div className="flex flex-col gap-1">
        <span className="text-start text-lg text-gsray-300 w-full">Stored DBC-Files:</span>
        <FileList files={files} />
      </div>
    </div>
  );
}

export default Dbc;
