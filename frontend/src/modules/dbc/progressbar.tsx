type Props = {
  used: number;
  total: number;
};

function ProgressBar({ used, total }: Props) {
  const progress = (100 / total) * used;
  return (
    <div className="relative w-80 bg-gray-200 rounded-xl h-12 dark:bg-gray-700">
      <div className="absolute w-80 left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2 ">
        <div className="flex flex-col items-center justify-center">
          <span className="text-sm font-medium text-black dark:text-gray-200">{`${progress.toFixed(1)}%`}</span>
          <span className="text-xs font-medium text-black dark:text-gray-300">{`${used}kB / ${total}kB`}</span>
        </div>
      </div>

      <div
        className="bg-purple-600 h-12 rounded-xl transition-all duration-1000 ease-in-out"
        style={{ width: `${progress > 6 ? progress : 6}%` }}
      ></div>
    </div>
  );
}

export default ProgressBar;
