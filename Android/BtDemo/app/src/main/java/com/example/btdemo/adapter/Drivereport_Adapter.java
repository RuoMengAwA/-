package com.example.btdemo.adapter;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.viewholder.BaseViewHolder;
import com.example.btdemo.R;
import com.example.btdemo.bean.ReportBean;

import java.util.List;

public class Drivereport_Adapter extends BaseQuickAdapter<ReportBean, BaseViewHolder> {

    public Drivereport_Adapter(int layoutResId , List<ReportBean> list) {
        super(layoutResId, list);
    }

    @Override
    protected void convert(BaseViewHolder holder, ReportBean reportBean) {
        String a[] = reportBean.getDateTime().split("T");
        String b[] = a[1].split("\\.");
        holder.setText(R.id.message,a[0]+"\n"+b[0] + "\n" + reportBean.getDangerousLevel());
//        holder.setText(R.id.time,reportBean.getNiceShareDate());
    }

}
